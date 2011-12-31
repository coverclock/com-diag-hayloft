/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/select.h>
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/s3/Action.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/Thread.h"
#include "com/diag/hayloft/CriticalSection.h"
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/Condition.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

static const Milliseconds RETRY = 1000;

static const Milliseconds TIMEOUT = 1000;

static const Milliseconds MINIMUM = 1000;

static const Epochalseconds BACKOFF = 1;

/*******************************************************************************
 * CLASS VARIABLES
 ******************************************************************************/

// To avoid deadlock, the locking order of the mutexen must be
// 1. Complex::instance,
// 2. Action::mutex,
// 3. Complex::shared,
// 4. Thread::mutex.

Mutex Complex::instance;

Mutex Complex::shared;

int Complex::instances = 0;

::com::diag::desperado::ticks_t Complex::numerator = 1;

::com::diag::desperado::ticks_t Complex::denominator = 1000;

Status Complex::status = ::S3StatusInternalError;

Pending * Complex::complex = 0;

::com::diag::hayloft::s3::LifeCycle * Complex::nextlifecycle = 0;

Complex::LifeCycle Complex::lifecycle;

Complex::Thread Complex::thread;

Complex::List Complex::starting;

Complex::List Complex::restarting;

Fibonacci Complex::fibonacci(1000);

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
 * COMPLEX
 ******************************************************************************/

// This will be called in the context of the Application Thread.
Complex::Complex()
{
	CriticalSection guard(instance);
	if (instances == 0) {
		::com::diag::desperado::Platform::instance().frequency(numerator, denominator);
		status = ::S3_create_request_context(&complex);
		nextlifecycle = &(LifeCycle::instance());
		LifeCycle::instance(lifecycle);
	}
	++instances;
	pending = complex;
	Logger & logger = Logger::instance();
	if (status != S3StatusOK) {
		logger.error("Complex@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, tostring(status));
	}
	logger.debug("Complex@%p: pending=%p\n", this, pending);
}

// This will be called in the context of the Application Thread.
Complex::~Complex() {
	CriticalSection guard(instance);
	--instances;
	if (instances == 0) {
		LifeCycle::instance(*nextlifecycle);
		thread.notify();
		thread.join();
		S3_destroy_request_context(complex);
		complex = 0;
		Action * action;
		::S3ErrorDetails errorDetails;
		memset(&errorDetails, 0, sizeof(errorDetails));
		for (Action * action = pop_front(starting); action != 0; action = pop_front(starting)) {
			CriticalSection guard(action->mutex);
			action->status = ::S3StatusInternalError;
			if (action->pending == complex) {
				action->condition.signal();
			}
			nextlifecycle->complete(*action, action->status, &errorDetails);
		}
		for (Action * action = pop_front(restarting); action != 0; action = pop_front(restarting)) {
			CriticalSection guard(action->mutex);
			action->status = ::S3StatusInternalError;
			if (action->pending == complex) {
				action->condition.signal();
			}
			nextlifecycle->complete(*action, action->status, &errorDetails);
		}
	}
}

// This may be called in the context if either Thread.
Action * Complex::pop_front(List & list) {
	CriticalSection guard(shared);
	Action * action = 0;
	if (!list.empty()) {
		action = list.front();
		list.pop_front();
	}
	return action;
}

// This may be called in the context if either Thread.
Complex::List & Complex::push_back(List & list, Action & action) {
	CriticalSection guard(shared);
	list.push_back(&action);
	return list;
}

// This will be called in the context of the Application Thread.
bool Complex::wait(Action & action) {
	CriticalSection guard(action.mutex);
	if (action.pending == complex) {
		while ((action.status == Action::PENDING) || (action.status == Action::BUSY)) {
			if (action.condition.wait(action.mutex) != 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

// This will be called in the context of the Application Thread.
bool Complex::start(Action & action) {
	CriticalSection guard(action.mutex);
	if (action.pending == complex) {
		if ((action.status != Action::PENDING) && (action.status != Action::BUSY)) {
			action.status = static_cast<Status>(Action::PENDING);
			push_back(starting, action);
			thread.start();
			return true;
		}
	}
	return false;
}

// This is always called from the context Complex Thread since it is invoked
// by libs3 as a result of the run method calling the libs3 iterate. There
// is no separate libs3 Thread (nor probably a cURL Thread).
void Complex::complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails) {
	CriticalSection guard(action.mutex);
	if (!::S3_status_is_retryable(final)) {
		alarm = 0;
		fibonacci.reset();
	} else if (action.pending != complex) {
		// Do nothing.
	} else if (action.reset()) {
		push_back(restarting, action);
		thread.start();
		return;
	} else {
		// Do nothing.
	}
	if (action.pending == complex) {
		action.condition.signal();

	}
	nextlifecycle->complete(action, final, errorDetails);
}

void * Complex::run() {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();

	logger.debug("Complex: begin\n");

	::com::diag::desperado::ticks_t retryticks = (RETRY * numerator) / (1000 * denominator);

	Seconds time;

	// This thread runs forever until notified by the last Complex destructor.

	while (!thread.notified()) {

		::com::diag::desperado::ticks_t delayticks = 0;

		do {

			// If the restarting list is not empty, then we've had Actions
			// with retryable failures. These are likely to be temporary
			// network failures. Recompute are back-off start time. Note
			// that only this thread access the restarting list.

			if (!restarting.empty()) {
				Epochalseconds delay = BACKOFF * fibonacci.next();
				logger.debug("Complex: backoff=%llus\n", delay);
				alarm = time.now() + delay;
			}

			// Process all Actions which are waiting for restarting by moving
			// them to the starting list. It's important to separate
			// these two states. It's possible that a retryable failure will
			// lead an Action being restarted to immediately complete. Putting
			// an Action in either state in the same list can lead to an
			// infinite loop as it continuously cycles back onto the starting
			// list, which would never become empty.

			for (Action * action = pop_front(restarting); action != 0; action = pop_front(restarting)) {
				CriticalSection guard(action->mutex);
				push_back(starting, *action);
			}

			// Wait until our back off alarm has gone off to start anything.

			if ((alarm == 0) || (alarm >= time.now())) {
				for (Action * action = pop_front(starting); action != 0; action = pop_front(starting)) {
					CriticalSection guard(action->mutex);
					if (!action->start()) {
						action->status = ::S3StatusInternalError; // Should never happen.
						action->condition.signal();
					}
				}
			}

			// Iterate once over any of the pending requests. This is necessary
			// just to get stuff started: sending any initial requests to S3 so
			// that we can then wait for the responses. As a side effect, we also
			// get a count on the number of pending Actions. Once this function
			// returns we assume that libs3 has do all the writes that can be done
			// at this point and is now waiting for S3 to respond so it can read.

			int actions = 0;
			Status result = ::S3_runonce_request_context(complex, &actions);
			if (result != S3StatusOK) {
				logger.error("Complex: S3_runonce_request_context failed! status=%d=\"%s\"\n", result, tostring(result));
				delayticks = retryticks;
				break;
			}

			// If there are no pending actions, might as well delay for a bit and
			// check again later.

			if (actions <= 0) {
				delayticks = retryticks;
				break;
			}

			logger.debug("Complex: actions=%d\n", actions);

			// We really only care about the reads, but we provide all three
			// fd_sets all zeroed out as required by libs3, even though we
			// won't use two of them.

			fd_set reads;
			fd_set writes;
			fd_set exceptions;

			FD_ZERO(&reads);
			FD_ZERO(&writes);
			FD_ZERO(&exceptions);

			int maxfd = -1;
			result = ::S3_get_request_context_fdsets(complex, &reads, &writes, &exceptions, &maxfd);
			if (result != ::S3StatusOK) {
				logger.error("Complex: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", result, tostring(result));
				delayticks = retryticks;
				break;
			}

			// If there are no open file descriptors (which seems unlikely if there
			// were pending actions), might was well delay for a bit and check
			// again later.

			if (maxfd < 0) {
				delayticks = retryticks;
				break;
			}

			logger.debug("Complex: maxfd=%d\n", maxfd);

			// Figure out a usable timeout and delay value. The libs3
			// ::S3_get_request_context_timeout function can return 0 of the
			// underlying cURL call fails. But the curl_multi_timeout function can
			// also return -1 indicating no suggested value, or 0 indicating don't
			// wait at all. Note that we don't use Fibonacci scaling here but
			// instead rely on the function to tell us the maximum delay.

			Milliseconds effective = TIMEOUT;
			Milliseconds maximum = ::S3_get_request_context_timeout(complex);
			if (effective > maximum) { effective = maximum; }
			if (effective < 0) { effective = MINIMUM; }

			// If we've been told not to wait at all, then there is something to
			// do right now. So we try again immediately (but we yield the processor
			// briefly so that in the event of something going pear shaped we don't
			// consume an entire CPU).

			if (effective == 0) {
				delayticks = 0;
				break;
			}

			logger.debug("Complex: timeout=%llums\n", effective);

			struct timeval timeoutval = { 0 };
			timeoutval.tv_sec = effective / 1000;
			timeoutval.tv_usec = (effective % 1000) * 1000;

			// No really, we only care about the reads. We assume if libs3 had
			// something to write it would do so. The fact that sockets are
			// writable without blocking isn't really useful to know. The real
			// purpose of this is just to delay this thread until there is something
			// for libs3 to do.

			int rc = ::select(maxfd, &reads, 0, 0, &timeoutval);
			if (rc < 0) {
				logger.error("Complex: select failed! error=%d=\"%s\"\n", errno, ::strerror(errno));
				delayticks = retryticks;
				break;
			}

			// This is inefficient but it is the only reliable way to do it
			// without cracking open the fd_set structure and relying on its
			// specific implementation.

::com::diag::desperado::Dump dump;
dump.words(&reads, sizeof(reads));

			int ready = 0;
			for (int fd = 0; fd <= maxfd; ++fd) {
				if (FD_ISSET(fd, &reads)) {
					++ready;
				}
			}

			// Nothing ready to read, still waiting on S3 to respond, might as well
			// wait a bit and then give libs3 a chance to process writes for any
			// actions that have come along since our last iteration.

			if (ready == 0) {
				delayticks = retryticks;
				break;
			}

			logger.debug("Complex: ready=%d\n", ready);

			delayticks = 0;

		} while (false);

		if (delayticks > 0) {
			platform.yield(delayticks);
		} else {
			Thread::yield();
		}

	}

	logger.debug("Complex: end\n");

	return 0;
}

}
}
}
}
