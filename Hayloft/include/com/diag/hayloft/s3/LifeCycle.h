/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_LIFECYCLE_
#define _H_COM_DIAG_HAYLOFT_S3_LIFECYCLE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Action;

/**
 * LifeCycle is an object that implements policy for significant events in the
 * life cycle of an Action: construction, start, S3 properties response, S3
 * completion, and destruction. LifeCycle can be used to implement complex
 * event handling, Action management, and instrumentation. There is only one
 * active LifeCycle in the system at a time, but different LifeCycle objects
 * can be instantiated and installed.
 */
class LifeCycle {

public:

	/**
	 * Allocates a reference to a new object of this type suitably initialized
	 * with default parameters.
	 *
	 * @return a reference to a new object of this type.
	 */
	static LifeCycle & factory();

	/**
	 * Establishes a new default instance of an object of this type.
	 *
	 * @param that refers to an object of this type.
	 * @return that.
	 */
	static LifeCycle & instance(LifeCycle & that);

    /**
     * Returns a reference to the default instance of an object of this type.
     *
     * @return a reference to the default instance of an object of this type.
     */
	static LifeCycle & instance();

protected:

    /**
     * Points to the default instance of this object.
     */
    static LifeCycle * singleton;

public:

	/**
	 * Ctor.
	 */
	explicit LifeCycle();

	/**
	 * Dtor.
	 */
	virtual ~LifeCycle();

	/**
	 * This method is called when the Action constructor is called.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overriding method does not need to call this method.
	 *
	 * Note that this method is called before any derived objects have been
	 * constructed.
	 *
	 * @param action refers to the Action being constructed.
	 */
	virtual void constructor(Action & action);

	/**
	 * This method is called when an Action is started. The Action will begin
	 * submitting requests to S3, receiving responses, and become complete.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overridding method does not need to call this method.
	 *
	 * Note that this method is called after the object is fully constructed.
	 *
	 * As a point of design, this method should be called just before the
	 * Action is actually started. Otherwise, when using the synchronous
	 * interface, you will find the complete method being called before the
	 * start method.
	 *
	 * @param action refers to the Action receiving the properties response.
	 */
	virtual void start(Action & action);

	/**
	 * This method is called when a Properties response is returned from S3.
	 * Hayloft guarantees that the default implementation does nothing except
	 * return ::S3StatusOK to indicate success. It can be safely overridden by
	 * the application for its own purposes. The overriding method does not need
	 * to call this method.
	 *
	 * @param action refers to the Action receiving the properties response.
	 * @param responseProperties points to a libs3 ::S3ResponseProperties
	 *        structure.
	 * @return a status that if ::S3StatusOK allows the Action to continue or
	 *         if anything else such as ::S3StatusAbortedByCallback immediately
	 *         terminates the Action.
	 */
	virtual Status properties(Action & action, const ::S3ResponseProperties * responseProperties);

	/**
	 * This method is called when libs3 completes executing the Action.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overridding method does not need to call this method. Hayloft guarantees
	 * that it will not make a reference to the Action after calling this
	 * method. Hence, this method may delete the Action providing the
	 * application is designed for it to do so. It can also restart the action,
	 * perhaps based on its status.
	 *
	 * @param action refers to the Action being completed.
	 * @param status is the final libs3 status for the Action. The status has
	 *        not yet been updated in the Action.
	 * @param errorDetails points to a libs3 ::S3ErrorDetails structure.
	 */
	virtual void complete(Action & action, Status final, const ::S3ErrorDetails * errorDetails);

	/**
	 * This method is called when the Action destructor is called.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overriding method does not need to call this method.
	 *
	 * Note that this method is called after any derived objects have already
	 * been destroyed.
	 *
	 * @param action refers to the Action being destroyed.
	 */
	virtual void destructor(Action & action);

};

}
}
}
}

#endif
