/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Region.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketTest : public Bucket {

private:

	char constraint[Region::LENGTH];

public:

	explicit BucketTest(const Session & se, const char * na, const Context & co = Context());

	explicit BucketTest(const Session & se, const char * na, Queue & qu, const Context & co = Context());

	virtual ~BucketTest();

	bool isInaccessible() const { return (status == ::S3StatusErrorAccessDenied); }

	bool isExistent() const { return (status == ::S3StatusOK); }

	bool isNonexistent() const { return (status == ::S3StatusErrorNoSuchBucket); }

protected:

	void test();

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

};

}
}
}
}

#endif
