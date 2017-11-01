/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/select.h>
#include "com/diag/hayloft/Complex.h"
#include "com/diag/hayloft/LifeCycle.h"
#include "com/diag/hayloft/Action.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/grandote/CriticalSection.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/target.h"
#include "com/diag/grandote/errno.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/stdio.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/*******************************************************************************
 * TUNABLE VARIABLES
 ******************************************************************************/

// These aren't constants, but they are fundamental tuning values. They
// can only be changed by overriding the base class and writing into them,
// for example during construction.

Milliseconds Complex::RETRY = 1000;

Milliseconds Complex::TIMEOUT = 1000;

Milliseconds Complex::BACKOFF = 1000;

Milliseconds Complex::MAXIMUM = 5000;

/*******************************************************************************
 * CLASS VARIABLES
 ******************************************************************************/

::com::diag::grandote::Platform * Complex::platform = 0;

Logger * Complex::logger = 0;

// To avoid deadlock, the locking order of the mutexen must be as follows.
// 1. Complex::instance
// 2. Action::mutex
// 3. Complex::shared
// 4. Thread::mutex
// But we try really hard not to have to lock mutexen in a nested fashion.

Mutex Complex::instance;

Mutex Complex::shared;

Condition Complex::ready;

int Complex::instances = 0;

::com::diag::grandote::ticks_t Complex::numerator = 0;

::com::diag::grandote::ticks_t Complex::denominator = 0;

Status Complex::status = ::S3StatusInternalError;

Handle * Complex::complex = 0;

::com::diag::hayloft::LifeCycle * Complex::nextlifecycle = 0;

Complex::LifeCycle Complex::lifecycle;

Complex::Thread Complex::thread;

Complex::List Complex::starting;

Complex::List Complex::restarting;

::com::diag::grandote::Fibonacci Complex::fibonacci(1000);

Epochalseconds Complex::alarm = 0;

/*******************************************************************************
 * LIFE CYCLE
 ******************************************************************************/

void Complex::LifeCycle::constructor(Action & action) {
	Complex::nextlifecycle->constructor(action);
}

void Complex::LifeCycle::start(Action & action) {
	Complex::nextlifecycle->start(action);
}

Status Complex::LifeCycle::properties(Action & action, const ::S3ResponseProperties * properties) {
	return Complex::nextlifecycle->properties(action, properties);
}

void Complex::LifeCycle::complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails) {
	Complex::complete(action, final, errorDetails);
}

void Complex::LifeCycle::destructor(Action & action) {
	Complex::nextlifecycle->destructor(action);
}

/*******************************************************************************
 * THREAD
 ******************************************************************************/

void * Complex::Thread::run() {
	return Complex::run();
}

/*******************************************************************************
 * COMPLEX PUBLIC API (APPLICATION THREAD CONTEXT)
 ******************************************************************************/

Complex::Complex()
{
	initialize();
	handle = complex;
	logger->debug("Complex@%p: handle=%p\n", this, handle);
}

Complex::~Complex() {
	finalize();
}

bool Complex::wait(Action & action) {
	return action.wait(complex);
}

bool Complex::start(Action & action) {
	bool result = false;
	if (action.startable(complex)) {
		thread.start();
		push_back_signal(starting, action);
		result = true;
	}
	return result;
}

/*******************************************************************************
 * COMPLEX UTILITIES
 ******************************************************************************/

void Complex::initialize() {
	::com::diag::grandote::CriticalSection guard(instance);
	if (instances == 0) {
		platform = &::com::diag::grandote::Platform::instance();
		logger = &Logger::instance();
		platform->frequency(numerator, denominator);
		status = ::S3_create_request_context(&complex);
		if (status != S3StatusOK) { logger->error("Complex: S3_create_request_context failed! status=%d=\"%s\"\n", status, tostring(status)); }
		nextlifecycle = &(LifeCycle::instance());
		LifeCycle::instance(lifecycle);
	}
	++instances;
}

void Complex::finalize() {
	::com::diag::grandote::CriticalSection guard(instance);
	--instances;
	if (instances == 0) {
		LifeCycle::instance(*nextlifecycle);
		thread.notify();
		ready.signal(); // Do we really need to lock shared for this?
		thread.join();
		S3_destroy_request_context(complex); // Forces all pending Actions to complete.
		complex = 0;
		Action * action;
		::S3ErrorDetails errorDetails = { 0 };
		for (Action * action = pop_front(starting); action != 0; action = pop_front(starting)) {
			nextlifecycle->complete(*action, ::S3StatusInterrupted, &errorDetails);
		}
		for (Action * action = pop_front(restarting); action != 0; action = pop_front(restarting)) {
			nextlifecycle->complete(*action, ::S3StatusInterrupted, &errorDetails);
		}
	}
}

// This may be called in the context of the Complex Thread.
Action * Complex::pop_front(List & list) {
	::com::diag::grandote::CriticalSection guard(shared);
	Action * action = 0;
	if (!list.empty()) {
		action = list.front();
		list.pop_front();
	}
	return action;
}

// This may be called in the context of the Complex Thread.
Complex::List & Complex::push_back(List & list, Action & action) {
	::com::diag::grandote::CriticalSection guard(shared);
	list.push_back(&action);
	return list;
}

// This may be called in the context of the Application Thread.
Complex::List & Complex::push_back_signal(List & list, Action & action) {
	::com::diag::grandote::CriticalSection guard(shared);
	list.push_back(&action);
	ready.signal();
	return list;
}

/*******************************************************************************
 * COMPLEX INTERNALS (COMPLEX THREAD CONTEXT)
 ******************************************************************************/

// This is always called from the context of the Complex Thread since it is
// invoked by libs3 as a result of the run method calling the libs3 iterate once
// function. So we don't have to be concerned about changing Complex variables
// that aren't shared with other Threads. We also don't have to bother
// signaling.
void Complex::complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails) {
	bool unretryable = true;
	if (action.restartable(final, unretryable, complex)) {
		push_back(restarting, action);
	} else {
		nextlifecycle->complete(action, final, errorDetails);
		if (unretryable) {
			alarm = 0;
			fibonacci.reset();
		}
	}
}

void * Complex::run() {
	Seconds time;

	logger->debug("Complex: begin\n");

	// This thread runs forever until notified by the last Complex destructor.

	while (!thread.notified()) {

		Milliseconds delay = RETRY;

		do {

			// The time is now!

			Epochalseconds now = time.now();

			// Process all Actions which are waiting for restarting by moving
			// them to the starting list. It's important to separate
			// these two states. It's possible that a retryable failure will
			// lead an Action being restarted to immediately complete. Putting
			// an Action in either state in the same list can lead to an
			// infinite loop as it continuously cycles back onto the starting
			// list, which would never become empty.

			bool restarted = false;
			for (Action * action = pop_front(restarting); action != 0; action = pop_front(restarting)) {
				logger->debug("Complex: Action@%p: restart\n", action);
				push_back(starting, *action);
				restarted = true;
			}

			// If the restarting list was not empty, then we've had Actions
			// with retryable failures. These are likely to be temporary
			// network failures. Recompute our back-off alarm time. If we
			// don't recompute our alarm time, it could be zero if we had no
			// failures or just had a success, or the prior alarm time if we
			// haven't had a success since the last retryable failure.

			if (restarted) {
				Epochalseconds backoff = ((BACKOFF  * fibonacci.next()) + 500) / 1000;
				alarm = now + backoff;
			}
			if (alarm <= now) {
				alarm = 0;
			}
			if (alarm > 0) {
				logger->debug("Complex: backoff=%llums\n", (alarm - now) * 1000);
			}

			// Wait until our back off alarm has gone off to start anything.
			// Then walk the list of starting Actions.

			if (alarm == 0) {
				::S3ErrorDetails errorDetails = { 0 };
				for (Action * action = pop_front(starting); action != 0; action = pop_front(starting)) {
					logger->debug("Complex: Action@%p: start\n", action);
					if (!action->start()) {
						logger->error("Complex: Action@%p: start failed!\n", action);
						nextlifecycle->complete(*action, ::S3StatusInternalError, &errorDetails);
					}
				}
			}

			// Iterate once over any of the pending requests. This is necessary
			// just to get stuff started: sending any initial requests to S3 so
			// that we can then wait for the responses. As a side effect, we
			// also get a count on the number of pending Actions. Once this
			// function returns we assume that libs3 has do all the writes that
			// can be done at this point and is now waiting for S3 to respond
			// so it can read.

			int active = 0;
			Status result = ::S3_runonce_request_context(complex, &active);
			if (result != S3StatusOK) {
				logger->error("Complex: S3_runonce_request_context failed! status=%d=\"%s\"\n", result, tostring(result));
				break;
			}

			// If there are no pending Actions, we might as well wait for
			// the Application to start one. We check for restarting Actions,
			// which only we can enqueue, as well as starting Actions. This can
			// happen if an error occurred in libs3 that caused an Action to go
			// straight from starting to complete, in which case it may be
			// restarting.

			if (active <= 0) {
				::com::diag::grandote::CriticalSection guard(shared);
				while (starting.empty() && restarting.empty() && (!thread.notified())) {
					int error = ready.wait(shared);
					if (error != 0) {
						logger->error("Complex: wait failed! error=%d=\"%s\"\n", error, ::strerror(error));
						break;
					} else {
						delay = 0;
					}
				}
				break;
			}
			logger->debug("Complex: active=%d\n", active);

			// See what file descriptors libs3 cares about.

			fd_set reads;
			fd_set writes;
			fd_set exceptions;

			FD_ZERO(&reads);
			FD_ZERO(&writes);
			FD_ZERO(&exceptions);

			int maxfd = -1;
			result = ::S3_get_request_context_fdsets(complex, &reads, &writes, &exceptions, &maxfd);
			if (result != ::S3StatusOK) {
				logger->error("Complex: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", result, tostring(result));
				break;
			}

			// If there are no open file descriptors (which seems unlikely if
			// there were pending actions), we need to give libs3 another chance
			// do some writes.

			if (maxfd < 0) {
				break;
			}
			logger->debug("Complex: maxfd=%d\n", maxfd);

			// Figure out a usable timeout and delay value. The libs3
			// ::S3_get_request_context_timeout function can return 0 if the
			// underlying cURL call fails. But the curl_multi_timeout function
			// can also return -1 indicating no suggested value, or 0 indicating
			// don't wait at all. Note that we don't use Fibonacci scaling here
			// but instead rely on the function to tell us the maximum delay.

			Milliseconds timeout = ::S3_get_request_context_timeout(complex);
			if (timeout < 0) { timeout = TIMEOUT; }
			if (timeout > MAXIMUM) { timeout = MAXIMUM; }

			// If cURL says not to wait at all, then there is something to do
			// right now. So we try again without delay.

			if (timeout == 0) {
				delay = 0;
				break;
			}
			logger->debug("Complex: timeout=%llums\n", timeout);

			struct timeval timeoutval = { 0 };
			timeoutval.tv_sec = timeout / 1000;
			timeoutval.tv_usec = (timeout % 1000) * 1000;

			// We copy libs3 and wait on all the FD sets.

			int rc = ::select(maxfd + 1, &reads, &writes, &exceptions, &timeoutval);
			if (rc < 0) {
				logger->error("Complex: select failed! error=%d=\"%s\"\n", errno, ::strerror(errno));
				break;
			}

			// This is inefficient but it is the only reliable way to do it
			// without cracking open the fd_set structure and relying on its
			// specific implementation.

			int readable = 0;
			int writable = 0;
			int exceptional = 0;
			for (int fd = 0; fd <= maxfd; ++fd) {
				if (FD_ISSET(fd, &reads)) {
					++readable;
				}
				if (FD_ISSET(fd, &writes)) {
					++writable;
				}
				if (FD_ISSET(fd, &exceptions)) {
					++exceptional;
				}
			}
			if ((readable == 0) && (writable == 0) && (exceptional==0)) {
				break;
			}
			logger->debug("Complex: readable=%d writable=%d exceptional=%d\n", readable, writable, exceptional);
			delay = 0;

		} while (false);

		// Platform guarantees that even if ticks is zero it will yield the
		// processor.

		::com::diag::grandote::ticks_t ticks = (delay * (numerator / 1000)) / denominator;
		logger->debug("Complex: delay=%llums=%lluticks\n", delay, ticks);
		platform->yield(ticks);

	}

	logger->debug("Complex: end\n");

	return 0;
}

}
}
}
