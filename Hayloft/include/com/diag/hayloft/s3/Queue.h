/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_QUEUE
#define _H_COM_DIAG_HAYLOFT_S3_QUEUE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "libs3.h"

struct S3RequestContext;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Queue {

public:

	typedef int64_t Milliseconds;

	/**
	 * This isn't really the default timeout value in milliseconds, which would
	 * be a long long time. But whatever timeout value is specified is limited
	 * in the underlying code to the value dynamically suggested by
	 * S3_get_request_context_timeout() via curl_multi_timeout() at run time.
	 * So using this value results in that dynamic maximum value being used.
	 */
	static const Milliseconds DEFAULT = intmaxof(Milliseconds);

	/**
	 * S3_get_request_context_timeout() calls curl_multi_timeout() which can
	 * return -1 indicating it has no suggested timeout value. The CURL docs
	 * recommend waiting at most a few seconds. This is this value we use when
	 * this happens.
	 */
	static const Milliseconds TIMEOUT = 1000;

	static int pendings;

private:

	::S3RequestContext * requests;

	::S3Status status;

public:

	explicit Queue();

	virtual ~Queue();

	operator bool() { return (status == ::S3StatusOK); }

	::S3RequestContext * getRequests() { return requests; }

	virtual bool all();

	virtual bool once(int & pending = pendings);

	virtual int ready(Milliseconds timeout = DEFAULT);

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	Queue(const Queue& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	Queue& operator=(const Queue& that);

};

}
}
}
}

#endif
