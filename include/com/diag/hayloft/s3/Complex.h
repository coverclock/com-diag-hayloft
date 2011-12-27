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

#include "com/diag/hayloft/s3/Plex.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

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

	Status status;

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

};

}
}
}
}

#endif
