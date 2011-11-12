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

BucketTest::BucketTest(const Session & se, const char * na, const Context & co)
: Bucket(se, na, co)
{
	region[0] = '\0';
	test();
}

BucketTest::BucketTest(const Session & se, const char * na, Queue & qu, const Context & co)
: Bucket(se, na, qu, co)
{
	region[0] = '\0';
	test();
}

BucketTest::~BucketTest() {
}

void BucketTest::test() {
	Logger::instance().debug("BucketTest@%p: begin\n", this);
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	::S3_test_bucket(
		context.getProtocol(),
		context.getStyle(),
		context.getId(),
		context.getSecret(),
		session.getHostName(),
		name.c_str(),
		sizeof(region), region,
		requests,
		&handler,
		this
	);
}

void BucketTest::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger & logger = Logger::instance();
	logger.debug("BucketTest@%p: end\n", this);
	Bucket::complete(s3status, errorDetails);
	if (region[0] != '\0') {
		logger.debug("BucketTest@%p: region=\"%s\"\n", this, region);
	}
}

}
}
}
}
