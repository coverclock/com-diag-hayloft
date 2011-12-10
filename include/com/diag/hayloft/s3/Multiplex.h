/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_MULTIPLEX_
#define _H_COM_DIAG_HAYLOFT_S3_MULTIPLEX_

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
 * Multiplex is how asynchronous Actions are managed. All Actions that use the
 * same Multiplex are executed in parallel as the Multiplex is serviced until
 * all Actions complete. If a Multiplex is deleted, all pending asynchronous
 * Actions on that Multiplex are immediately aborted and completed with a
 * failure status and are automatically converted into synchronous Actions.
 * Deleting an asynchronous Action that is BUSY and pending on a Multiplex
 * causes it and all other pending Actions on that Multiplex to be forced
 * to complete (there is no mechanism though which a single Action can be
 * forced to complete).
 */
class Multiplex : public Plex {

protected:

	::S3Status status;

public:

	/**
	 * Ctor. A new libs3 S3RequestContext is dynamically allocated.
	 */
	explicit Multiplex();

	/**
	 * Dtor. The S3RequestContext is deleted. If there are pending Actions,
	 * the dtor blocks and they are all forced to complete.
	 */
	virtual ~Multiplex();

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
	::S3Status getStatus() const { return status; }

private:

	void initialize();

};

}
}
}
}

#endif
