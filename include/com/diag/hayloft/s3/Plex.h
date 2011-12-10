/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_PLEX_
#define _H_COM_DIAG_HAYLOFT_S3_PLEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/select.h>
#include "com/diag/hayloft/types.h"
#include "com/diag/desperado/generics.h"
#include "libs3.h"

struct S3RequestContext;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Plex is how asynchronous Actions are managed. All Actions that use the
 * same Plex are executed in parallel as the Plex is serviced until
 * all Actions complete. Deleting an asynchronous Action that is BUSY and
 * pending on a Plex causes it and all other pending Actions on that Plex to be
 * forced to complete (there is no mechanism though which a single Action can be
 * forced to complete).
 */
class Plex {

public:

	/**
	 * This defines the meaning of the bits returned when iterating and
	 * servicing.
	 */
	enum Ready {
		READY		= (1 << 0),						/* Sockets are READY */
		PENDING		= (1 << 1),						/* Actions are PENDING */
		RETRY		= (1 << 2),						/* Failed but RETRYable */
		ERROR		= (1 << (widthof(int) - 1))		/* Failed with ERROR */
	};

	/**
	 * This isn't really the default timeout value in milliseconds, which would
	 * be a long long time. But whatever timeout value is specified is limited
	 * in the underlying code to the value dynamically suggested by
	 * S3_get_request_context_timeout() via curl_multi_timeout() at run time.
	 * So using this value results in that dynamic maximum value being used.
	 */
	static const Milliseconds TIMEOUT = intmaxof(Milliseconds);

	/**
	 * This is the default LIMIT, which is a really big number. But the
	 * service() method only iterates while their are PENDING Actions and
	 * while there are READY sockets. It would be quite the application that
	 * hit this LIMIT.
	 */
	static const int LIMIT = intmaxof(int);

	/**
	 * S3_get_request_context_timeout() calls curl_multi_timeout() which can
	 * return -1 indicating it has no suggested timeout value. The CURL docs
	 * recommend waiting at most a few seconds. This is this value we use when
	 * this happens.
	 */
	static const Milliseconds DEFAULT = 1000;

	/**
	 * This variable cares about pending Actions even when the caller does not.
	 */
	static int dontcare;

protected:

	::S3RequestContext * requests;

public:

	/**
	 * Ctor. Specifying a NULL S3RequestContext, which is the default, results
	 * in Actions that are synchronous.
	 *
	 * @param requestContext is the S3RequestContext. It is UNTAKEN and must
	 *        be deallocated by the application.
	 */
	explicit Plex(::S3RequestContext * requestContext = 0 /* UNTAKEN */);

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
	::S3RequestContext * getRequests() const { return requests; }

	/**
	 * Force all Actions on this Multiplex to run to completion.
	 *
	 * @return a mask of Ready bits containing ERROR or RETRY.
	 */
	virtual int complete();

	/**
	 * Perform a single iteration of all Actions on this Multiplex
	 *
	 * @param pending refers to a variable into which the number of pending
	 *        Actions is stored.
	 * @return a mask of Ready bits containing ERROR, RETRY, or PENDING.
	 */
	virtual int iterate(int & pending = dontcare);

	/**
	 * Perform a select(2) system call on any sockets being mananged by libs3
	 * and return a mask of Ready bits indicating what their state is.
	 *
	 * @param timeout is a timeout duration in milliseconds.
	 * @return a mask of Ready bits containing ERROR, RETRY, or READY.
	 */
	virtual int ready(Milliseconds timeout = TIMEOUT);

	/**
	 * Wait for ready() sockets and iterate() on any pending Actions. Simply
	 * calling iteratively until PENDING is not returned is functionally
	 * equivalent to complete() (ideally zero would be returned if no errors
	 * occurred). Returning with PENDING and READY means there is work to do
	 * and unprocessed responses available from S3. Returning with PENDING but
	 * not READY means there is still work to do but we're waiting on S3 to
	 * respond.
	 *
	 * @param timeout is a timeout duration in milliseconds to wait for any
	 *        interesting sockets to become READY.
	 * @param limit is the maximum number of iterations to do as long as Actions
	 *        are PENDING and sockets are READY.
	 * @return a mask of Ready bits containing ERROR, RETRY, PENDING, or READY.
	 */
	virtual int service(Milliseconds timeout = TIMEOUT, int limit = LIMIT);

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
}

#endif
