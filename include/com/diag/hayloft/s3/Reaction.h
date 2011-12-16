/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_REACTION_
#define _H_COM_DIAG_HAYLOFT_S3_REACTION_

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
 * Reaction is an object that implements optional call back methods for
 * significant events in the life cycle of an Action: construction, S3
 * properties response, S3 completion, and destruction. Reaction can be used
 * to implement complex event handling, Action management, and instrumentation,
 * via dependency injection.
 */
class Reaction {

public:

	/**
	 * Ctor.
	 */
	explicit Reaction();

	/**
	 * Dtor.
	 */
	virtual ~Reaction();

	/*
	 * Why no constructor method? The Reaction is established by dependency
	 * injection using a settor after (perhaps long after) the Action is
	 * constructed.
	 */

	/*
	 * Why no start method? The Reaction is established by dependency injection
	 * using a settor after the Action is constructed, so it's principle use is
	 * to manage an asynchronous Action. Asynchronous actions are always
	 * explicitly started by the application either directly or via the complete
	 * method.
	 */

	/**
	 * This method is called when a Properties response is returned from S3.
	 * Hayloft guarantees that the default implementation does nothing except
	 * return ::S3StatusOK to indicate success. It can be safely overridden by
	 * the application for its own purposes. The overriding method does not need
	 * to call this method. If this method terminates the Action by returning
	 * something other than ::S3StatusOK, it is free to delete the Action
	 * providing the application is designed for it to do so.
	 *
	 * @param action refers to the Action receiving the properties response.
	 * @param properties points to a libs3 ::S3ResponseProperties structure.
	 * @return a status that if ::S3StatusOK allows the Action to continue or
	 *         if anything else (like ::S3StatusAbortedByCallback) immediates
	 *         aborts the Action.
	 */
	virtual Status properties(Action & action, const ::S3ResponseProperties * properties);

	/**
	 * This method is called when libs3 completes executing the Action.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overridding method does not need to call this method. This method is
	 * free to delete the Action providing the application is designed for it
	 * to do so. It can also restart the action, perhaps based on its status.
	 *
	 * @param action refers to the Action being completed.
	 * @param errorDetails points to a libs3 ::S3ErrorDetails structure.
	 */
	virtual void complete(Action & action, const ::S3ErrorDetails * errorDetails);

	/**
	 * This method is called when the Action destructor is called.
	 * Hayloft guarantees that the default implementation does nothing. It can
	 * be safely overridden by the application for its own purposes. The
	 * overriding method does not need to call this method. Note that this
	 * method is called after any derived objects have been destroyed.
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
