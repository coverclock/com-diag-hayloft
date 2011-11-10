/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Context.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session;

class BucketValid {

private:

	::S3Status status;

public:

	explicit BucketValid(Session & session, const char * name, const Context & context = Context());

	virtual ~BucketValid() {}

	operator bool() { return (status == ::S3StatusOK); }

	::S3Status getStatus() const { return status; }

};


}
}
}
}

#endif
