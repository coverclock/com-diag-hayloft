/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST_

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
#include "com/diag/hayloft/s3/Session.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketTest : public Bucket {

private:

	char constraint[Region::LENGTH];

public:

	explicit BucketTest(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit BucketTest(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~BucketTest();

	bool isInaccessible() const { return (status == ::S3StatusErrorAccessDenied); }

	bool isExistent() const { return (status == ::S3StatusOK); }

	bool isNonexistent() const { return (status == ::S3StatusErrorNoSuchBucket); }

protected:

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

	void begin();

};

}
}
}
}

#endif
