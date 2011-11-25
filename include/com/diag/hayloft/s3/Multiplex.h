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

#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "libs3.h"

struct S3RequestContext;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Multiplex {

public:

	typedef int32_t Bits;

	enum Ready {
		READ		= (1 << 0),
		WRITE		= (1 << 1),
		EXCEPTION	= (1 << 2),
		PENDING		= (1 << 3),
		RETRY		= (1 << (widthof(Bits) - 2)),
		ERROR		= (1 << (widthof(Bits) - 1))
	};

	typedef int64_t Milliseconds;

	/**
	 * This isn't really the default timeout value in milliseconds, which would
	 * be a long long time. But whatever timeout value is specified is limited
	 * in the underlying code to the value dynamically suggested by
	 * S3_get_request_context_timeout() via curl_multi_timeout() at run time.
	 * So using this value results in that dynamic maximum value being used.
	 */
	static const Milliseconds TIMEOUT = intmaxof(Milliseconds);

	static const int LIMIT = intmaxof(int);

	/**
	 * S3_get_request_context_timeout() calls curl_multi_timeout() which can
	 * return -1 indicating it has no suggested timeout value. The CURL docs
	 * recommend waiting at most a few seconds. This is this value we use when
	 * this happens.
	 */
	static const Milliseconds DEFAULT = 1000;

	static int dontcare;

protected:

	::S3RequestContext * requests;

	::S3Status status;

public:

	explicit Multiplex();

	virtual ~Multiplex();

	operator bool() { return (status == ::S3StatusOK); }

	::S3Status getStatus() const { return status; }

	::S3RequestContext * getRequests() const { return requests; }

	virtual bool complete();

	virtual bool iterate(int & pending = dontcare);

	virtual Bits ready(Milliseconds timeout = TIMEOUT);

	virtual Bits service(Milliseconds timeout = TIMEOUT, int limit = LIMIT);

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	Multiplex(const Multiplex& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	Multiplex& operator=(const Multiplex& that);

};

}
}
}
}

#endif
