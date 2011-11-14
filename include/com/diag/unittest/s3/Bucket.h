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
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/BucketValid.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketTest.h"
#include "com/diag/hayloft/s3/BucketDelete.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture BucketBaseTest;

TEST_F(BucketBaseTest, Heap) {
	Session session;
	Bucket * bucket = new Bucket(session, "BucketBaseTestHeap");
	ASSERT_NE(bucket, (Bucket*)0);
	EXPECT_TRUE((*bucket) == true);
	EXPECT_FALSE(bucket->isBusy());
	EXPECT_FALSE(bucket->isRetryable());
	EXPECT_EQ(bucket->getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket->getName(), (char *)0);
	delete bucket;
}

TEST_F(BucketBaseTest, Stack) {
	Session session;
	Bucket bucket(session, "BucketBaseTestStack");
	EXPECT_TRUE(bucket == true);
	EXPECT_FALSE(bucket.isBusy());
	EXPECT_FALSE(bucket.isRetryable());
	EXPECT_EQ(bucket.getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket.getName(), (char *)0);
}

TEST_F(BucketBaseTest, Temporary) {
	Session session;
	EXPECT_TRUE(Bucket(session, "BucketBaseTestTemporary") == true);
}

typedef Fixture BucketValidTest;

TEST_F(BucketValidTest, Heap) {
	Session session;
	BucketValid * valid = new BucketValid(session, "BucketValidTestHeap");
	ASSERT_NE(valid, (BucketValid*)0);
	EXPECT_TRUE((*valid) == true);
	EXPECT_TRUE(valid->isValid());
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	Session session;
	BucketValid valid(session, "BucketValidTestStack");
	EXPECT_TRUE(valid == true);
	EXPECT_TRUE(valid.isValid());
}

typedef Fixture BucketInitialTest;

TEST_F(BucketInitialTest, Heap) {
	Session session;
	BucketTest * test = new BucketTest(session, "BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isExistent());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketInitialTest, Stack) {
	Session session;
	BucketTest test(session, "BucketCreateTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketInitialTest, Explicit) {
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketTest test(session, "BucketCreateTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketInitialTest, All) {
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestAll", queue);
	EXPECT_TRUE(queue.all());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketInitialTest, Once) {
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestOnce", queue);
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = queue.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

typedef Fixture BucketCreateTest;

TEST_F(BucketCreateTest, Heap) {
	Session session;
	BucketCreate * test = new BucketCreate(session, "BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isCreated());
	delete test;
}

TEST_F(BucketCreateTest, Stack) {
	Session session;
	BucketCreate test(session, "BucketCreateTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isCreated());
}

TEST_F(BucketCreateTest, Explicit) {
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketCreate test(session, "BucketCreateTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isCreated());
}

TEST_F(BucketCreateTest, All) {
	Session session;
	Queue queue;
	BucketCreate test(session, "BucketCreateTestAll", queue);
	EXPECT_TRUE(queue.all());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isCreated());
}

TEST_F(BucketCreateTest, Once) {
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	Session session;
	Queue queue;
	BucketCreate test(session, "BucketCreateTestOnce", queue);
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = queue.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isCreated());
}

typedef Fixture BucketTestTest;

TEST_F(BucketTestTest, Heap) {
	Session session;
	BucketTest * test = new BucketTest(session, "BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_TRUE(test->isExistent());
	EXPECT_FALSE(test->isNonexistent());
	delete test;
}

TEST_F(BucketTestTest, Stack) {
	Session session;
	BucketTest test(session, "BucketCreateTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

TEST_F(BucketTestTest, Explicit) {
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketTest test(session, "BucketCreateTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

TEST_F(BucketTestTest, All) {
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestAll", queue);
	EXPECT_TRUE(queue.all());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

TEST_F(BucketTestTest, Once) {
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestOnce", queue);
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = queue.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

typedef Fixture BucketDeleteTest;

TEST_F(BucketDeleteTest, Heap) {
	Session session;
	BucketDelete * test = new BucketDelete(session, "BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isDeleted());
	delete test;
}

TEST_F(BucketDeleteTest, Stack) {
	Session session;
	BucketDelete test(session, "BucketCreateTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isDeleted());
}

TEST_F(BucketDeleteTest, Explicit) {
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketDelete test(session, "BucketCreateTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isDeleted());
}

TEST_F(BucketDeleteTest, All) {
	Session session;
	Queue queue;
	BucketDelete test(session, "BucketCreateTestAll", queue);
	EXPECT_TRUE(queue.all());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isDeleted());
}

TEST_F(BucketDeleteTest, Once) {
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	Session session;
	Queue queue;
	BucketDelete test(session, "BucketCreateTestOnce", queue);
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = queue.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isDeleted());
}

typedef Fixture BucketVerifyTest;

TEST_F(BucketVerifyTest, Heap) {
	Session session;
	BucketTest * test = new BucketTest(session, "BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isExistent());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketVerifyTest, Stack) {
	Session session;
	BucketTest test(session, "BucketCreateTestStack");
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketVerifyTest, Explicit) {
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketTest test(session, "BucketCreateTestExplicit", context);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketVerifyTest, All) {
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestAll", queue);
	EXPECT_TRUE(queue.all());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketVerifyTest, Once) {
	static const int LIMIT = 100;
	static const Queue::Milliseconds TIMEOUT = 1000;
	Session session;
	Queue queue;
	BucketTest test(session, "BucketCreateTestOnce", queue);
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = queue.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
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
