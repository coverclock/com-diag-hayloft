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
#include "com/diag/hayloft/Fibonacci.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/string.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/*******************************************************************************
 * CLASS VARIABLES
 ******************************************************************************/

Mutex Complex::mutex;

int Complex::instances = 0;

::com::diag::desperado::ticks_t Complex::numerator = 1;

::com::diag::desperado::ticks_t Complex::denominator = 1000;

Status Complex::status = ::S3StatusInternalError;

Pending * Complex::complex = 0;

::com::diag::hayloft::s3::LifeCycle * Complex::nextlifecycle = 0;

Complex::LifeCycle Complex::lifecycle;

Complex::Thread Complex::thread;

Complex::List Complex::list;

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

Complex::Complex()
{
	CriticalSection guard(mutex);
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

Complex::~Complex() {
	CriticalSection guard(mutex);
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
		while (!list.empty()) {
			action = list.front();
			list.pop_front();
			nextlifecycle->complete(*action, ::S3StatusAbortedByCallback, &errorDetails);
		}
	}
}

bool Complex::start(Action & action) {
	Action * actionable = 0;
	{
		CriticalSection guard(action.mutex);
		if (action.pending == complex) {
			if ((action.status != Action::PENDING) && (action.status != Action::BUSY) && (action.status != Action::FINAL)) {
				action.status = static_cast<Status>(Action::PENDING);
				actionable = &action;
			}
		}
	}
	if (actionable != 0) {
		CriticalSection guard(mutex);
		list.push_back(actionable);
		thread.start();
		return true;
	} else {
		return false;
	}
}

bool Complex::wait(Action & action) {
	CriticalSection guard(action.mutex);
	if (action.pending != complex) {
		return false;
	} else {
		while ((action.status != Action::PENDING) && (action.status != Action::BUSY) && (action.status != Action::FINAL) && (::S3_status_is_retryable(action.status) != 0)) {
			if (action.condition.wait(action.mutex) != 0) {
				return false;
			}
		}
		return true;
	}
}

void Complex::complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails) {
	{
		CriticalSection guard(action.mutex);
		if (action.pending == complex) {
			if (::S3_status_is_retryable(action.status) != 0) {
				CriticalSection guard(mutex);
				list.push_back(&action);
				thread.start();
				return;
			}
			action.condition.signal();
		}
	}
	// The LifeCycle complete method or the Action complete method that it
	// calls is permitted to delete the Action. So there can be no references
	// to any fields in the Action following this call. That includes its mutex.
	nextlifecycle->complete(action, final, errorDetails);
}

static const Milliseconds RETRY = 1000;
static const Milliseconds TIMEOUT = 1000;
static const Milliseconds MINIMUM = 1000;

void * Complex::run() {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();

	logger.debug("Complex: begin\n");

	::com::diag::desperado::ticks_t retryticks = (RETRY * numerator) / (1000 * denominator);

	int actions;

	Status result;

	fd_set reads;
	fd_set writes;
	fd_set exceptions;

	int maxfd;

	Milliseconds maximum;
	Milliseconds effective;

	struct timeval timeoutval = { 0 };

	int rc;

	int ready;

	::com::diag::desperado::ticks_t delayticks = 0;

	// This thread runs forever until notified by the last Complex destructor.

	while (!thread.notified()) {

		do {

			// Step through everything on our list and see what needs to be done.

			Action * action;
			do {
				{
					CriticalSection guard(mutex);
					if (!list.empty()) {
						action = list.front();
						list.pop_front();
					} else {
						action = 0;
					}
				}
				if (action != 0) {
					// Insert backoff stuff here.
					action->start();
				}
			} while (action != 0);

			// Iterate once over any of the pending requests. This is necessary
			// just to get stuff started: sending any initial requests to S3 so
			// that we can then wait for the responses. As a side effect, we also
			// get a count on the number of pending Actions. Once this function
			// returns we assume that libs3 has do all the writes that can be done
			// at this point and is now waiting for S3 to respond so it can read.

			actions = 0;
			result = ::S3_runonce_request_context(complex, &actions);
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

			FD_ZERO(&reads);
			FD_ZERO(&writes);
			FD_ZERO(&exceptions);

			maxfd = -1;
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

			logger.debug("Complex: fd=%d\n", maxfd);

			// Figure out a usable timeout and delay value. The libs3
			// ::S3_get_request_context_timeout function can return 0 of the
			// underlying cURL call fails. But the curl_multi_timeout function can
			// also return -1 indicating no suggested value, or 0 indicating don't
			// wait at all. Note that we don't use Fibonacci scaling here but
			// instead rely on the function to tell us the maximum delay.

			effective = TIMEOUT;
			maximum = ::S3_get_request_context_timeout(complex);
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

			timeoutval.tv_sec = effective / 1000;
			timeoutval.tv_usec = (effective % 1000) * 1000;

			// No really, we only care about the reads. We assume if libs3 had
			// something to write it would do so. The fact that sockets are
			// writable without blocking isn't really useful to know. The real
			// purpose of this is just to delay this thread until there is something
			// for libs3 to do.

			rc = ::select(maxfd, &reads, 0, 0, &timeoutval);
			if (rc < 0) {
				logger.error("Complex: select failed! error=%d=\"%s\"\n", errno, ::strerror(errno));
				delayticks = retryticks;
				break;
			}

			// This is really inefficient but it is the only reliable way to do it
			// without cracking open the fd_set structure and relying on its
			// specific implementation.

			ready = 0;
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
