/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_PLEX_
#define _H_COM_DIAG_HAYLOFT_PLEX_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

class Action;

/**
 * Plex is how asynchronous Actions are managed. All Actions that use the
 * same Plex are executed in parallel as the Plex is serviced until
 * all Actions complete. Deleting an asynchronous Action that is BUSY and
 * pending on a Plex causes it and all other pending Actions on that Plex to be
 * forced to complete. There is no mechanism through which a single Action can
 * be forced to complete independent of other Actions on the same Plex. All
 * Actions on the same Plex may share the same socket connection to S3.
 */
class Plex {

protected:

	Handle * handle;

public:

	/**
	 * Ctor. Specifying a NULL S3RequestContext, which is the default, results
	 * in Actions that are synchronous.
	 *
	 * @param untaken is the S3RequestContext. It is UNTAKEN and must be
	 *        destroyed by the application.
	 */
	explicit Plex(Handle * untaken = 0 /* UNTAKEN */);

	/**
	 * Dtor. If there are pending Actions, the dtor blocks and they are all
	 * forced to complete.
	 */
	virtual ~Plex();

	/**
	 * Get the S3RequestContext.
	 *
	 * @return the S3RequestContext.
	 */
	Handle * getHandle() const { return handle; }

private:

	void initialize();

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Plex(const Plex & that);

    /**
     *  Assignment operator. POISONED..
     *
     *  @param that refers to an R-value object of this type.
     */
	Plex & operator=(const Plex & that);

};

}
}
}

#endif
