/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_COMPLEX_
#define _H_COM_DIAG_HAYLOFT_S3_COMPLEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <list>
#include "com/diag/hayloft/s3/Plex.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/Thread.h"
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/Condition.h"
#include "com/diag/hayloft/Fibonacci.h"
#include "com/diag/hayloft/Seconds.h"
#include "com/diag/hayloft/s3/S3.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/desperado/types.h"

namespace com {
namespace diag {
namespace desperado {
class Platform;
}
}
}

namespace com {
namespace diag {
namespace hayloft {
class Logger;
}
}
}

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Action;

/**
 * Complex is system-wide mechanism to manage Actions. It implements a separate
 * thread of control to service started Actions as a background activity,
 * automatically re-starting retryable Actions that failed due to network
 * errors. It ties into the system Life Cycle facility to be notified of
 * completed Actions, which eliminates any need for it to cache references to
 * Actions. All Actions that use the same Complex are executed in parallel as
 * the Complex thread executes. If a Complex is deleted, all pending
 * Actions on that Complex are immediately aborted and completed with a failure
 * status and are automatically converted into synchronous Actions. Deleting an
 * asynchronous Action that is BUSY and pending on a Complex causes it and all
 * other pending Actions on that Complex to be forced to complete (there is no
 * mechanism though which a single Action can be forced to complete).
 */
class Complex : public Plex {

protected:

	struct LifeCycle : public ::com::diag::hayloft::s3::LifeCycle {

		virtual void constructor(Action & action);

		virtual void start(Action & action);

		virtual Status properties(Action & action, const ::S3ResponseProperties * properties);

		virtual void complete(Action & action, Status status, const ::S3ErrorDetails * errorDetails);

		virtual void destructor(Action & action);

	};

	struct Thread : public ::com::diag::hayloft::Thread {

		virtual void * run();

	};

	typedef std::list<Action *> List;

	static ::com::diag::desperado::Platform * platform;

	static Logger * logger;

	/**
	 * If we have an internal error, like an error return on a system call,
	 * for which there is no known recovery, we assume it is a temporary
	 * condition, we wait for this many milliseconds, and iterate again.
	 */
	static Milliseconds RETRY;

	/**
	 * If cURL via libs3 doesn't propose a timeout period for waiting one a
	 * socket to become readable, this is how many milliseconds we wait.
	 */
	static Milliseconds TIMEOUT;

	/**
	 * If cURL proposes a timeout period longer than this many milliseconds
	 * (it routinely suggests almost as long as thirty seconds), we wait this
	 * many milliseconds instead. Waiting for less time than cURL suggests is
	 * okay, we just can't wait longer or else its timers will pop.
	 */
	static Milliseconds MAXIMUM;

	/**
	 * If we get a failure, our back off duration is some multiple of this many
	 * milliseconds.
	 */
	static Milliseconds BACKOFF;

	/**
	 * This is the maximum number of times an Action may be retried before
	 * deciding we're not getting anywhere.
	 */
	static int RETRIES;

private:

	static Mutex instance;

	static Mutex shared;

	static Condition ready;

	static int instances;

	static ::com::diag::desperado::ticks_t numerator;

	static ::com::diag::desperado::ticks_t denominator;

	static Status status;

	static Pending * complex;

	static ::com::diag::hayloft::s3::LifeCycle * nextlifecycle;

	static LifeCycle lifecycle;

	static Thread thread;

	static List starting;

	static List restarting;

	static Fibonacci fibonacci;

	static Epochalseconds alarm;

protected:

	static void * run();

	static void complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails);

	static Action * pop_front(List & list);

	static List & push_back(List & list, Action & action);

	static List & push_back_signal(List & list, Action & action);

public:

	/**
	 * Ctor.
	 */
	explicit Complex();

	/**
	 * Dtor.
	 */
	virtual ~Complex();

	/**
	 * Returns true if construction was successful.
	 *
	 * @return true if construction was successful.
	 */
	operator bool() const { return (status == ::S3StatusOK); }

	/**
	 * Get the status associated with construction.
	 *
	 * @return the status.
	 */
	Status getStatus() const { return status; }

	/**
	 * Submit the Action to the Complex service thread for processing. Possible
	 * failure modes are: the referenced Action did not reference a Complex
	 * object as its Plex during construction; or the Action is not in a state
	 * in which it can be started.
	 *
	 * @param action refers to the Action to be submitted.
	 * @return true if successful, false otherwise.
	 */
	virtual bool start(Action & action);

	/**
	 * Block the calling thread until the referenced Action is complete.
	 * Possible failure modes are: the referenced Action did not reference a
	 * Complex object as its Plex during construction; or the Action is not in
	 * a state during which it can be waited upon. When this call returns
	 * successfully, the referenced Action is guaranteed to have a final
	 * status which is not retryable.
	 *
	 * @param action refers to the Action to be waited upon.
	 * @return true if successful, false otherwise.
	 */
	virtual bool wait(Action & action);

};

}
}
}
}

#endif
