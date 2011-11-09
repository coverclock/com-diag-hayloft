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
#include "com/diag/desperado/Platform.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture BucketValidTest;

TEST_F(BucketValidTest, Heap) {
	Session session;
	Credentials credentials(Environment::getAccessKeyId(), Environment::getSecretAccessKey());
	Context context(credentials);
	BucketValid * valid = new BucketValid(session, "BucketValidTestHeap", context);
	ASSERT_NE(valid, (BucketValid*)0);
	EXPECT_TRUE((*valid) == true);
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	Session session;
	Credentials credentials(Environment::getAccessKeyId(), Environment::getSecretAccessKey());
	Context context(credentials);
	BucketValid valid(session, "BucketValidTestStack", context);
	EXPECT_TRUE(valid == true);
}

typedef Fixture BucketTestTest;

TEST_F(BucketTestTest, Stack) {
	Session session;
	Credentials credentials(Environment::getAccessKeyId(), Environment::getSecretAccessKey());
	Context context(credentials);
	BucketTest test(session, "BucketTestTestStack", context);
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	for (int ii = 0; (ii < 10) && (test == false); ++ii) {
		platform.yield(platform.frequency());
	}
}

}
}
}
}

#endif
