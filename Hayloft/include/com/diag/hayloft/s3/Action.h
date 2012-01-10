/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ACTION_
#define _H_COM_DIAG_HAYLOFT_S3_ACTION_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/Condition.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Plex;
class Reaction;
class LifeCycle;
class Complex;

/**
 * Action is a C++ object whose state may be altered in the background by a
 * libs3 thread and which presents an asynchronous interface in which the
 * Action has to be explicitly started by the application. Actions can also
 * be used synchronously so that this Action is started and completed all within
 * the constructor of the derived class.
 */
class Action {

	friend class LifeCycle;

	friend class Complex;

public:

	/**
	 * This is a fake libs3 ::S3Status value that means this Action has a
	 * transitional status between constructing and starting.
	 */
	static const int IDLE = intmaxof(Status);

	/**
	 * This is a fake libs3 ::S3Status value that means this Action has a
	 * transitional status between starting or restarting and running.
	 */
	static const int PENDING = intmaxof(Status) - 1;

	/**
	 * This is a fake libs3 ::S3Status value that means this Action has a
	 * transitional status between running and completing.
	 */
	static const int BUSY = intmaxof(Status) - 2;

	/**
	 * This is a fake libs3 ::S3Status value that means this Action has a
	 * transitional status between completing and restarting.
	 */
	static const int FINAL = intmaxof(Status) - 3;

	/**
	 * This is the maximum number of times an Action may be retried before
	 * deciding we're not getting anywhere. It does not include the initial
	 * try.
	 */
	static const int RETRIES = 4;

protected:

	static bool dontcare;

	static Status responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData);

	static void responseCompleteCallback(Status final, const ::S3ErrorDetails * errorDetails, void * callbackData);

	std::string server;

	std::string requestid;

	std::string requestid2;

	Handle * handle;

	mutable Mutex mutex;

	Condition condition;

	int retries;

	::S3ResponseHandler handler;

private:

	Status status;

public:

	/***************************************************************************
	 * APPLICATION API
	 **************************************************************************/

	/**
	 * Ctor. Use this for the synchronous interface. The Action will be
	 * automatically started in the ctor, and the ctor will block until the
	 * Action completes. Synchronous Actions can be re-started and will
	 * re-execute synchronously.
	 *
	 * This constructor calls the corresponding LifeCycle method.
	 */
	explicit Action();

	/**
	 * Ctor. Use this for the asynchronous interface. The application is
	 * responsible for starting this Action, and for insuring that the Plex
	 * associated with this Action is serviced until this Action completes.
	 * Asynchronous Actions can be restarted and re-executed asynchronously.
	 *
	 * This constructor calls the corresponding LifeCycle method.
	 *
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction. However the Plex dtor will force all Actions
	 *        pending on the Plex to complete.
	 */
	explicit Action(const Plex & plex);

	/**
	 * Dtor.
	 *
	 * This destructor calls the corresponding LifeCycle method.
	 */
	virtual ~Action();

	/**
	 * Return true if this Action has achieved a final (non-transitional) state.
	 * @return true if this Action has achieved a final state.
	 */
	operator bool() const { Status status = state(); return ((status != IDLE) && (status != BUSY) && (status != PENDING)  && (status != FINAL)); }

	/**
	 * Return true if this Action is in a state in which it has never been
	 * started.
	 *
	 * @return return true if this Action is IDLE.
	 */
	bool isIdle() const { return (state() == IDLE); }

	/**
	 * Return true if this Action is in a state in which it cannot be started
	 * or reset.
	 *
	 * @return return true if this Action is BUSY.
	 */
	bool isBusy() const { Status status = state(); return ((status == PENDING) || (status == BUSY) || (status == FINAL)); }

	/**
	 * Return true if this Action is in a state that is retryable, that is, in
	 * a state that is a result of a temporary failure and in which this Action
	 * is likely to succeed if it is restarted. Transitional status values,
	 * ::S3StatusOK which indicates success, and "permanent" failure status
	 * values such as ::S3StatusHttpErrorNotFound, are not retryable.
	 *
	 * @return return true if this Action is in a state that is retryable,
	 */
	bool isRetryable() const { return (::S3_status_is_retryable(state()) != 0); }

	/**
	 * Return true if this Action has the status ::S3StatusOK which generally
	 * indicates succcess.
	 *
	 * @return true if this Action indicates success.
	 */
	bool isSuccessful() const { return (state() == ::S3StatusOK); }

	/**
	 * Return true if this Action has a status that indicates the requested
	 * S3 resource exists but is inaccessible. Typically this is because this
	 * Action does not have permission to access the resource.
	 *
	 * @return true if this Action indicates inaccessibility.
	 */
	bool isInaccessible() const { Status status = state(); return ((status == ::S3StatusHttpErrorForbidden) || (status == ::S3StatusErrorAccessDenied)); }

	/**
	 * Return true if this Action has a status that indicates the requested
	 * S3 resource does not exist.
	 *
	 * @return true if this Action indicates nonexistence.
	 */
	bool isNonexistent() const { Status status = state(); return ((status == ::S3StatusHttpErrorNotFound) || (status == ::S3StatusErrorNoSuchKey) || (status == ::S3StatusErrorNoSuchBucket)); }

	/**
	 * Get the libs3 ::S3RequestContext associated with this Action. This
	 * value is derived from the Plex used during construction.
	 *
	 * @return the libs3 ::S3RequestContext associated with this Action.
	 */
	Handle * getHandle() const { return handle; }

	/**
	 * Get the status for this Action. A pointer to a C string describing the
	 * status can be optionally returned.
	 *
	 * @param description if non-null points to a variable into which a pointer
	 *        to a C string describing the status is returned.
	 * @return the status.
	 */
	Status getStatus(const char ** description = 0) const;

	/**
	 * Get the name of the S3 server used to execute this Action. This is
	 * less useful than it sounds, since I have never seen S3 return any
	 * name other than "AmazonS3".
	 *
	 * @return the name S3 server used to execute this Action.
	 */
	const char * getServer() const { return server.c_str(); }

	/**
	 * Get the request ID used to execute this Action.
	 *
	 * @return the request ID used to execute this Action.
	 */
	const char * getRequestId() const { return requestid.c_str(); }

	/**
	 * Get the request ID 2 used to execute this Action.
	 *
	 * @return the request ID 2 used to execute this Action..
	 */
	const char * getRequestId2() const { return requestid2.c_str(); }

	/**
	 * Start this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool start();

	/**
	 * Reset this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset();

	/**
	 * Block the calling thread until this Action is signaled to be complete.
	 * This method is only useful if the Application executes Actions in a
	 * background thread.
	 *
	 * @param candidate if non-null causes this method to fail if this Action's
	 *        handle is not equal to the candidate value.
	 * @return true if successful, false otherwise.
	 */
	virtual bool wait(Handle * candidate = 0);

	/***************************************************************************
	 * MANAGEMENT API
	 **************************************************************************/

	/**
	 * This returns the current status value with an appropriate memory barrier
	 * to accommodate threads running on different processors.
	 *
	 * @return the current status value.
	 */
	Status state() const;

	/**
	 * This updates the current status and returns its prior value with an
	 * appropriate memory barrier to accommodate threads running on different
	 * processors.
	 *
	 * @param update is the new status value.
	 * @return the prior status value.
	 */
	Status state(Status update);

	/**
	 * Determine if this Action is retryable given the specified status. This
	 * method takes a broader view of retryability to include not only
	 * temporary network failures but possibly convergence latency too. Whether
	 * this is reasonable for all Actions is up to the derived class, which is
	 * permitted to override this method to implement an alternative strategy.
	 *
	 * @param final refers to the Status to be evaluated for retryability.
	 *        This is not the status that is part of this Action because this
	 *        decision needs to be made prior to committing a new status
	 *        value to the status field where it will be visible to
	 *        polling threads.
	 * @param nonexistence is the value returned for statuses indicating
	 *        non-existence.
	 * @return true if retryable, false otherwise.
	 */
	virtual bool retryable(Status final, bool nonexistence = true);

	/**
	 * Determine if this Action is startable, and if so prepare it to be
	 * started.
	 *
	 * @param candidate if non-null causes this method to fail if this Action's
	 *        handle is not equal to the candidate value.
	 * @return true if startable, false otherwise.
	 */
	virtual bool startable(Handle * candidate = 0);

	/**
	 * Determine if this Action is restartable, and if so prepare it to be
	 * restarted.
	 *
	 * @param final refers to the Status to be evaluated for restartability.
	 *        This is not the status that is part of this Action because this
	 *        decision needs to be made prior to committing a new status
	 *        value to the status field where it will be visible to
	 *        polling threads.
	 * @param unretryable if true is a clue to the caller that this Action is
	 *        not restartable because it completed with a unretryable status.
	 *        The caller may choose to alter its strategy depending on this
	 *        value.
	 * @param candidate if non-null causes this method to fail if this Action's
	 *        handle is not equal to the candidate value.
	 * @return true if restartable, false otherwise.
	 */
	virtual bool restartable(Status final, bool & unretryable = dontcare, Handle * candidate = 0);

	/**
	 * Signal any waiting threads that this Action is complete.
	 *
	 * @param final is the value to which the status is set.
	 * @return true if successful, false otherwise.
	 */
	virtual bool signal(Status final);

protected:

	/**
	 * This method is called when a Properties response is returned from S3.
	 *
	 * The default implementation in the base class returns ::S3StatusOK to
	 * allow this Action to continue. If the overriding method returns a status
	 * other than ::S3StatusOK (like ::S3StatusAbortedByCallback) this Action
	 * will be immediately terminated and completed with that status.
	 *
	 * @param responseProperties points to a libs3 ::S3ResponseProperties
	 *        structure.
	 * @return a status that if ::S3StatusOK allows this Action to continue or
	 *         if anything else such as ::S3StatusAbortedByCallback immediately
	 *         terminates this Action.
	 */
	virtual Status properties(const ::S3ResponseProperties * responseProperties);

	/**
	 * This method is called when libs3 completes executing this Action.
	 *
	 * The default implementation in the base class does nothing. The overriding
	 * method is allowed to delete this Action (providing the application is
	 * otherwise designed for it to do so); Hayloft guarantees it will make
	 * no further reference to it following this call. The overriding method is
	 * allowed to restart this Action.
	 *
	 * @param final is the final libs3 status.
	 * @param errorDetails points to a libs3 ::S3ErrorDetails structure.
	 */
	virtual void complete(Status final, const ::S3ErrorDetails * errorDetails);

	void execute();

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  It isn't safe to sythetically copy an Action (for either the L-value or
     *  the R-value) while it may be being modified by a background thread.
     *  Subclasses such as Object and Bucket provide their own copy ctors.
     *
     *  @param that refers to an R-value object of this type.
     */
	Action(const Action& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  It isn't safe to sythetically copy an Action (for either the L-value or
     *  the R-value) while it may be being modified by a background thread.
     *
     *  @param that refers to an R-value object of this type.
     */
	Action& operator=(const Action& that);

	void initialize();

};

}
}
}
}

#endif
