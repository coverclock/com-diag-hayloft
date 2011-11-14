/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETCREATE
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETCREATE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketCreate : public Bucket {


public:

	explicit BucketCreate(const Session & se, const char * na, const Context & co = Context());

	explicit BucketCreate(const Session & se, const char * na, Queue & qu, const Context & co = Context());

	virtual ~BucketCreate();

	bool isCreated() const { return (status == ::S3StatusOK); }

protected:

	void initialize();

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

};

}
}
}
}

#endif
