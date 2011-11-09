/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_BUCKET
#define _H__COM_DIAG_UNITTEST_S3_BUCKET

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture BucketValidTest;

TEST_F(BucketValidTest, Heap) {
	Session session;
	BucketValid * valid = new BucketValid(session, "BucketValidTestHeap");
	ASSERT_NE(valid, (BucketValid*)0);
	EXPECT_TRUE((*valid) == true);
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	Session session;
	BucketValid valid(session, "BucketValidTestStack");
	EXPECT_TRUE(valid == true);
}

typedef Fixture BucketTestTest;

TEST_F(BucketTestTest, Stack) {
	Session session;
	Credentials credentials(Environment::getAccessKeyId(), Environment::getSecretAccessKey());
	LocationConstraint constraint;
	ProtocolUnsecure protocol;
	UniversalResourceIdentifierStylePath style;
	Context context(credentials, constraint, protocol, style);
	BucketTest test(session, "BucketTestTestStack", context);
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	int ii;
	for (ii = 0; (ii < 10000) && (test == false); ++ii) {
		platform.yield(platform.frequency() / 1000);
	}
	EXPECT_TRUE(test == true);
	Logger::instance().debug("%d ms elapsed\n", ii);
}

}
}
}
}

#endif
