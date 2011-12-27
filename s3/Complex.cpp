/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/select.h>
#include <list>
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
 * TYPES
 ******************************************************************************/

typedef std::list<Action *> List;

/*******************************************************************************
 * LIFE CYCLE DECLARATION
 ******************************************************************************/

class ComplexLifeCycle : public LifeCycle {

public:

	explicit ComplexLifeCycle();

	virtual ~ComplexLifeCycle();

	virtual void constructor(Action & action);

	virtual void start(Action & action);

	virtual Status properties(Action & action, const ::S3ResponseProperties * properties);

	virtual void complete(Action & action, Status status, const ::S3ErrorDetails * errorDetails);

	virtual void destructor(Action & action);

};

/*******************************************************************************
 * ACTION THREAD DECLARATION
 ******************************************************************************/

class ComplexActionThread : public Thread {

private:

	const Milliseconds minimum;

	const Milliseconds maximum;

public:

	explicit ComplexActionThread(
		Milliseconds minimumms = 1000,
		Milliseconds maximumms = 120000
	);

	virtual ~ComplexActionThread();

	virtual void * run();

};

/*******************************************************************************
 * SERVICE THREAD DECLARATION
 ******************************************************************************/

class ComplexServiceThread : public Thread {

private:

	const Milliseconds retry;

	const Milliseconds timeout;

	const Milliseconds minimum;

public:

	explicit ComplexServiceThread(
		Milliseconds retryms = 1000,
		Milliseconds timeoutms = 1000,
		Milliseconds minimumms = 1000
	);

	virtual ~ComplexServiceThread();

	virtual void * run();

};

/*******************************************************************************
 * STATIC AND CLASS VARIABLES
 ******************************************************************************/

static Mutex instancemutex;

static int instances = 0;

static int actions = 0;

static int active = 0;

static LifeCycle * lifecycle = 0;

static ComplexLifeCycle * complexlifecycle = 0;

static ComplexServiceThread * servicethread = 0;

static ComplexActionThread * actionthread = 0;

static Status complexstatus = ::S3StatusOK;

static Pending * complexpending = 0;

static ::com::diag::desperado::ticks_t numerator = 1;

static ::com::diag::desperado::ticks_t denominator = 1000;

static Mutex actionmutex;

static List actionlist;

static Condition actionavailable;

static Fibonacci fibonacci;

/*******************************************************************************
 * LIFE CYCLE IMPLEMENTATION
 ******************************************************************************/

ComplexLifeCycle::ComplexLifeCycle()
{}

ComplexLifeCycle::~ComplexLifeCycle() {}

void ComplexLifeCycle::constructor(Action & action) {
	lifecycle->constructor(action);
}

void ComplexLifeCycle::start(Action & action) {
	lifecycle->start(action);
}

Status ComplexLifeCycle::properties(Action & action, const ::S3ResponseProperties * properties) {
	return lifecycle->properties(action, properties);
}

void ComplexLifeCycle::complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails) {
	if (action.getPending() == complexpending) {
		CriticalSection guard(actionmutex);
		if (::S3_status_is_retryable(final) != 0) {
			actionlist.push_back(&action);
			actionavailable.signal();
			return;
		} else {
			fibonacci.reset();
		}
	}
	lifecycle->complete(action, final, errorDetails);
}

void ComplexLifeCycle::destructor(Action & action) {
	lifecycle->destructor(action);
}

/*******************************************************************************
 * ACTION THREAD IMPLEMENTATION
 ******************************************************************************/

ComplexActionThread::ComplexActionThread(Milliseconds minimumms, Milliseconds maximumms)
: minimum(minimumms)
, maximum(maximumms)
{
}

ComplexActionThread::~ComplexActionThread() {
}

void * ComplexActionThread::run() {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();

	while (true) {

		Action * action;
		unsigned int factor;

		{
			CriticalSection guard(actionmutex);
			while (actionlist.empty() && (!notified())) {
				actionavailable.wait(actionmutex);
			}
			if (notified()) { break; }
			action = actionlist.front();
			actionlist.pop_front();
			factor = fibonacci.get();
			if (factor < 1000) { factor = fibonacci.next(); }
		}

		Milliseconds delay = minimum * factor;
		if (delay > maximum) { delay = maximum; }
		logger.debug("Complex: delay=%llums\n", delay);

		::com::diag::desperado::ticks_t retryticks = (delay * numerator) / (1000 * denominator);
		platform.yield(retryticks);

		action->reset();
		action->start();

	}

	return 0;
}

/*******************************************************************************
 * SERVICE THREAD IMPLEMENTATION
 ******************************************************************************/

ComplexServiceThread::ComplexServiceThread(Milliseconds retryms, Milliseconds timeoutms, Milliseconds minimumms)
: retry(retryms)
, timeout(timeoutms)
, minimum(minimumms)
{}

ComplexServiceThread::~ComplexServiceThread() {}

void * ComplexServiceThread::run() {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();

	::com::diag::desperado::ticks_t retryticks = (retry * numerator) / (1000 * denominator);

	int pending;

	Status status;

	fd_set reads;
	fd_set writes;
	fd_set exceptions;

	int maxfd;

	Milliseconds maximum;
	Milliseconds effective;

	struct timeval timeoutval = { 0 };

	int rc;

	int ready;

	// This thread runs forever until notified by the last Complex destructor.

	while (!notified()) {

		// Iterate once over any of the pending requests. This is necessary
		// just to get stuff started: sending any initial requests to S3 so
		// that we can then wait for the responses. As a side effect, we also
		// get a count on the number of pending Actions. Once this function
		// returns we assume that libs3 has do all the writes that can be done
		// at this point and is now waiting for S3 to respond so it can read.

		pending = 0;
		status = ::S3_runonce_request_context(complexpending, &pending);
		if (status != S3StatusOK) {
			logger.error("Complex: S3_runonce_request_context failed! status=%d=\"%s\"\n", status, tostring(status));
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
		}

		// If there are no pending actions, might as well delay for a bit and
		// check again later.

		if (pending <= 0) {
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
		}

		logger.debug("Complex: pending=%d\n", pending);

		// We really only care about the reads, but we provide all three
		// fd_sets all zeroed out as required by libs3, even though we
		// won't use two of them.

		FD_ZERO(&reads);
		FD_ZERO(&writes);
		FD_ZERO(&exceptions);

		maxfd = -1;
		status = ::S3_get_request_context_fdsets(complexpending, &reads, &writes, &exceptions, &maxfd);
		if (status != ::S3StatusOK) {
			logger.error("Complex: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", status, tostring(status));
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
		}

		// If there are no open file descriptors (which seems unlikely if there
		// were pending actions), might was well delay for a bit and check
		// again later.

		if (maxfd < 0) {
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
		}

		logger.debug("Complex: fd=%d\n", maxfd);

		// Figure out a usable timeout and delay value. The libs3
		// ::S3_get_request_context_timeout function can return 0 of the
		// underlying cURL call fails. But the curl_multi_timeout function can
		// also return -1 indicating no suggested value, or 0 indicating don't
		// wait at all. Note that we don't use Fibonacci scaling here but
		// instead rely on the function to tell us the maximum delay.

		effective = timeout;
		maximum = ::S3_get_request_context_timeout(complexpending);
		if (effective > maximum) { effective = maximum; }
		if (effective < 0) { effective = minimum; }

		// If we've been told not to wait at all, then there is something to
		// do right now. So we try again immediately (but we yield the processor
		// briefly so that in the event of something going pear shaped we don't
		// consume an entire CPU).

		if (effective == 0) {
			if (!notified()) {
				yield();
			}
			continue;
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
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
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
			if (!notified()) {
				platform.yield(retryticks);
			}
			continue;
		}

		logger.debug("Complex: ready=%d\n", ready);

	}

	return 0;
}

/*******************************************************************************
 * PLEX DECLARATION
 ******************************************************************************/

Complex::Complex()
{
	CriticalSection guard(instancemutex);
	if (instances == 0) {
		::com::diag::desperado::Platform::instance().frequency(numerator, denominator);
		complexpending = 0;
		complexstatus = ::S3_create_request_context(&complexpending);
		lifecycle = &(LifeCycle::instance());
		complexlifecycle = new ComplexLifeCycle;
		LifeCycle::instance(*complexlifecycle);
		servicethread = new ComplexServiceThread;
		actionthread = new ComplexActionThread;
	}
	++instances;
	actionthread->start();
	servicethread->start();
	pending = complexpending;
	status = complexstatus;
}

Complex::~Complex() {
	CriticalSection guard(instancemutex);
	--instances;
	if (instances == 0) {
		LifeCycle::instance(*lifecycle);
		delete complexlifecycle;
		complexlifecycle = 0;
		S3_destroy_request_context(complexpending);
		complexpending = 0;
		servicethread->notify();
		servicethread->join();
		delete servicethread;
		servicethread = 0;
		actionthread->notify();
		actionavailable.signal();
		actionthread->join();
		delete actionthread;
		actionthread = 0;
		Action * action;
		::S3ErrorDetails errorDetails;
		memset(&errorDetails, 0, sizeof(errorDetails));
		while (!actionlist.empty()) {
			action = actionlist.front();
			actionlist.pop_front();
			lifecycle->complete(*action, ::S3StatusAbortedByCallback, &errorDetails);
		}
	}
}

}
}
}
}
