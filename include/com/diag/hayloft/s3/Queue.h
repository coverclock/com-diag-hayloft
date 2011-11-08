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

#include "libs3.h"

struct S3RequestContext;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Queue {

public:

	explicit Queue();

	virtual ~Queue();

private:

	::S3RequestContext * context;

	::S3Status status;

public:

	operator bool() { return (status == ::S3StatusOK); }

	::S3RequestContext * getRequestContext() { return context; }

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
