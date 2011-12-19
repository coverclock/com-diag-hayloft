/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ACTION_
#define _H_COM_DIAG_HAYLOFT_S3_ACTION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Plex;
class Reaction;

/**
 * Action is a C++ object whose state may be altered in the background by a
 * libs3 thread and which presents an asynchronous interface in which the
 * Action has to be explicitly started by the application. Actions can also
 * be used synchronously so that the Action is started and completed all within
 * the constructor of the derived class.
 */
class Action {

public:

	/**
	 * This is a fake libs3 ::S3Status value that means the Action has been
	 * started and is being executed in the background by a separate libs3
	 * thread. Most methods which mutate an Action are ignored when the Action
	 * is in this state.
	 */
	static const int BUSY = intmaxof(Status);

	/**
	 * This is a fake libs3 ::S3Status value that means the Action has been
	 * constructed but has never been started.
	 */
	static const int IDLE = intmaxof(Status) - 1;

private:

	static Status responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData);

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string server;

	std::string requestid;

	std::string requestid2;

	Pending * pending;

	Status status;

	::S3ResponseHandler handler;

	/**
	 * This returns the current status within the context of a full global
	 * memory barrier with both read and write semantics that is performed both
	 * before the fetch and again afterwards. The fact that I use a memory
	 * barrier instead of a full mutex means I assume that the status value
	 * can be read and written atomically.
	 *
	 * @return	the current status value.
	 */
	Status state() const { ::com::diag::desperado::MemoryBarrier barrier; return status; }

public:

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
	 * responsible for starting the Action, and for insuring that the Plex
	 * associated with the Action is serviced until the Action completes.
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
	 * Return true if this Action is neither IDLE nor BUSY.
	 * @return true if this Action is neither IDLE nor BUSY.
	 */
	operator bool() const { Status temporary = state(); return ((temporary != IDLE) && (temporary != BUSY)); }

	/**
	 * Return true if this Action is IDLE.
	 *
	 * @return return true if this Action is IDLE.
	 */
	bool isIdle() const { return (state() == IDLE); }

	/**
	 * Return true if this Action is BUSY.
	 *
	 * @return return true if this Action is BUSY.
	 */
	bool isBusy() const { return (state() == BUSY); }

	/**
	 * Return true if this Action is in a state that is retryable, that is, in
	 * a state that is a result of a temporary failure and in which the Action
	 * is likely to succeed if it is restarted. Status values IDLE, BUSY,
	 * ::S3StatusOK which indicates success, and common failure codes such as
	 * ::S3StatusHttpErrorNotFound, are not retryable.
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
	bool isInaccessible() const { Status temporary = state(); return ((temporary == ::S3StatusHttpErrorForbidden) || (temporary == ::S3StatusErrorAccessDenied)); }

	/**
	 * Return true if this Action has a status that indicates the requested
	 * S3 resource does not exist.
	 *
	 * @return true if this Action indicates nonexistence.
	 */
	bool isNonexistent() const { Status temporary = state(); return ((temporary == ::S3StatusHttpErrorNotFound) || (temporary == ::S3StatusErrorNoSuchKey) || (temporary == ::S3StatusErrorNoSuchBucket)); }

	/**
	 * Get the libs3 ::S3RequestContext associated with this Action. This
	 * value is derived from the Plex used during construction.
	 *
	 * @return the libs3 ::S3RequestContext associated with this Action.
	 */
	Pending * getPending() const { return pending; }

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
	 * Start the Action.
	 *
	 * The default implementation in the base class does nothing. It is the
	 * responsibility of the derived classes to call the appropriate LifeCycle
	 * method when an Action is actually started. (As a point of design, you
	 * should call it just before the Action is actually started.) In practice,
	 * they can call the non-virtual execute function in this base class to
	 * accomplish this. The execute function is non-virtual since it may be
	 * called from a constructor when the synchronous interface is used, at
	 * which time construction of the object may not be complete (depending on
	 * the C++ implementation).
	 */
	virtual void start();

	/**
	 * Reset the action.
	 *
	 * The default implementation in the base class does nothing. Derived
	 * classes may use this to perform some recovery or reinitialization action
	 * when signaled by a management entity that calls this method before
	 * restarting the Action. Typically, actions that use input or output
	 * functors will do whatever is necessary to rewind the input or output
	 * stream.
	 */
	virtual void reset();

protected:

	/**
	 * This method is called when a Properties response is returned from S3.
	 *
	 * The default implementation in the base class calls the corresponding
	 * LifeCycle method. The default implementation of the corresponding
	 * LifeCycle method does nothing, but the default implementation may be
	 * replaced in order to implement more complex Action life cycle policies.
	 * Derived classes may override this method but is so doing the designer
	 * must decide whether to call this base class method thereby calling the
	 * LifeCycle method implicitly, call the LifeCycle method explicitly, or
	 * not call the LifeCycle method at all.
	 *
	 * If the overriding method returns a status other than ::S3StatusOK (like
	 * ::S3StatusAbortedByCallback) this Action will be immediately terminated
	 * and completed with that status.
	 *
	 * @param properties points to a libs3 ::S3ResponseProperties structure.
	 * @return a status that if ::S3StatusOK allows the Action to continue or
	 *         if anything else such as ::S3StatusAbortedByCallback immediately
	 *         terminates the Action.
	 */
	virtual Status properties(const ::S3ResponseProperties * properties);

	/**
	 * This method is called when libs3 completes executing the Action.
	 *
	 * The default implementation in the base class calls the corresponding
	 * LifeCycle method. The default implementation of the corresponding
	 * LifeCycle method does nothing, but the default implementation may be
	 * replaced in order to implement more complex Action life cycle policies.
	 * Derived classes may override this method but is so doing the designer
	 * must decide whether to call this base class method thereby calling the
	 * LifeCycle method implicitly, call the LifeCycle method explicitly, or
	 * not call the LifeCycle method at all.
	 *
	 * The overriding method can delete this Action providing the application
	 * is otherwise designed for it to do so. The default behavior of Hayloft
	 * is to not reference this Action following this call.
	 *
	 * The current Action status is not passed to this method but can be
	 * retrieved using the getStatus() method.
	 *
	 * @param errorDetails points to a libs3 ::S3ErrorDetails structure.
	 */
	virtual void complete(const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

protected:

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

};

}
}
}
}

#endif
