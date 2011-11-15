/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketTest.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketTest::BucketTest(const Session & session, const char * bucketname, const Context & context)
: Bucket(session, bucketname, context)
{
	initialize();
}

BucketTest::BucketTest(const Session & session, const char * bucketname, Queue & queue, const Context & context)
: Bucket(session, bucketname, queue, context)
{
	initialize();
}

BucketTest::~BucketTest() {
}

void BucketTest::initialize() {
	Logger::instance().debug("BucketTest@%p: begin\n", this);
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	constraint[0] = '\0';
	::S3_test_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		hostname.c_str(),
		name.c_str(),
		sizeof(constraint), constraint,
		requests,
		&handler,
		this
	);
}

void BucketTest::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("BucketTest@%p: end\n", this);
	region = constraint;
}

}
}
}
}
