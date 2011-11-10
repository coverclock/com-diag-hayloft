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
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Status.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/generics.h"

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

TEST_F(BucketTestTest, Heap) {
	Session session;
	BucketTest * test = new BucketTest(session, "BucketTestTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isExistent());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketTestTest, Stack) {
	Session session;
	BucketTest test(session, "BucketTestTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketTestTest, Explicit) {
	Session session;
	Credentials credentials;
	LocationConstraint constraint;
	Protocol protocol;
	UniversalResourceIdentifierStyle style;
	CannedAccessControlList list;
	Context context(credentials, constraint, protocol, style, list);
	BucketTest test(session, "BucketTestTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketTestTest, Asynchronous) {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	int limit;
	int pending;
	int ready;
	Session session;
	Queue queue;
	BucketTest test(session, "BucketTestTestAsynchronous", queue);
	for (limit = 0, pending = 1, ready = 0; (limit < LIMIT) && (test != true) && (ready >= 0) && (pending > 0); ++limit) {
		ready = queue.ready(TIMEOUT);
		EXPECT_TRUE(ready >= 0);
		EXPECT_TRUE(queue.once(pending));
		platform.yield();
	}
	EXPECT_EQ(ready, 0);
	EXPECT_EQ(pending, 0);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

}
}
}
}

#endif
