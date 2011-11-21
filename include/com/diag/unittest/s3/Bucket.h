/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_BUCKET_
#define _H__COM_DIAG_UNITTEST_S3_BUCKET_

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
	Bucket * bucket = new Bucket("BucketBaseTestHeap");
	ASSERT_NE(bucket, (Bucket*)0);
	EXPECT_TRUE((*bucket) == true);
	EXPECT_FALSE(bucket->isBusy());
	EXPECT_FALSE(bucket->isRetryable());
	EXPECT_EQ(bucket->getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket->getName(), (char *)0);
	delete bucket;
}

TEST_F(BucketBaseTest, Stack) {
	Bucket bucket("BucketBaseTestStack");
	EXPECT_TRUE(bucket == true);
	EXPECT_FALSE(bucket.isBusy());
	EXPECT_FALSE(bucket.isRetryable());
	EXPECT_EQ(bucket.getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket.getName(), (char *)0);
}

TEST_F(BucketBaseTest, Temporary) {
	EXPECT_TRUE(Bucket("BucketBaseTestTemporary") == true);
}

typedef Fixture BucketValidTest;

TEST_F(BucketValidTest, Heap) {
	BucketValid * valid = new BucketValid("BucketValidTestHeap");
	ASSERT_NE(valid, (BucketValid*)0);
	EXPECT_TRUE((*valid) == true);
	EXPECT_TRUE(valid->isValid());
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	BucketValid valid("BucketValidTestStack");
	EXPECT_TRUE(valid == true);
	EXPECT_TRUE(valid.isValid());
}

typedef Fixture BucketInitialTest;

TEST_F(BucketInitialTest, Heap) {
	BucketTest * test = new BucketTest("BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isExistent());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketInitialTest, Stack) {
	BucketTest test("BucketCreateTestStack");
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
	BucketTest test("BucketCreateTestExplicit", context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketInitialTest, Complete) {
	Multiplex multiplex;
	BucketTest test("BucketCreateTestComplete", multiplex);
	test.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketInitialTest, Service) {
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	BucketTest test("BucketCreateTestService", multiplex);
	test.start();
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
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
	BucketCreate * test = new BucketCreate("BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isCreated());
	delete test;
}

TEST_F(BucketCreateTest, Stack) {
	BucketCreate test("BucketCreateTestStack");
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
	BucketCreate test("BucketCreateTestExplicit", context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isCreated());
}

TEST_F(BucketCreateTest, Complete) {
	Multiplex multiplex;
	BucketCreate create("BucketCreateTestComplete", multiplex);
	create.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(create == true);
	EXPECT_FALSE(create.isBusy());
	EXPECT_FALSE(create.isRetryable());
	EXPECT_TRUE(create.isCreated());
}

TEST_F(BucketCreateTest, Service) {
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	BucketCreate create("BucketCreateTestService", multiplex);
	create.start();
	int bits = 0;
	for (int limit = LIMIT; (create != true) && (limit > 0); --limit) {
		if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(create == true);
	EXPECT_FALSE(create.isBusy());
	EXPECT_FALSE(create.isRetryable());
	EXPECT_TRUE(create.isCreated());
}

typedef Fixture BucketTestTest;

TEST_F(BucketTestTest, Heap) {
	BucketTest * test = new BucketTest("BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_TRUE(test->isExistent());
	EXPECT_FALSE(test->isNonexistent());
	delete test;
}

TEST_F(BucketTestTest, Stack) {
	BucketTest test("BucketCreateTestStack");
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
	BucketTest test("BucketCreateTestExplicit", context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

TEST_F(BucketTestTest, Complete) {
	Multiplex multiplex;
	BucketTest test("BucketCreateTestComplete", multiplex);
	test.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isExistent());
	EXPECT_FALSE(test.isNonexistent());
}

TEST_F(BucketTestTest, Service) {
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	BucketTest test("BucketCreateTestService", multiplex);
	test.start();
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
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
	BucketDelete * test = new BucketDelete("BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isDeleted());
	delete test;
}

TEST_F(BucketDeleteTest, Stack) {
	BucketDelete test("BucketCreateTestStack");
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
	BucketDelete test("BucketCreateTestExplicit", context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isDeleted());
}

TEST_F(BucketDeleteTest, Complete) {
	Multiplex multiplex;
	BucketDelete remove("BucketCreateTestComplete", multiplex);
	remove.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(remove == true);
	EXPECT_FALSE(remove.isBusy());
	EXPECT_FALSE(remove.isRetryable());
	EXPECT_TRUE(remove.isDeleted());
}

TEST_F(BucketDeleteTest, Service) {
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	BucketDelete remove("BucketCreateTestService", multiplex);
	remove.start();
	int bits = 0;
	for (int limit = LIMIT; (remove != true) && (limit > 0); --limit) {
		if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
	}
	EXPECT_EQ(bits, 0);
	EXPECT_TRUE(remove == true);
	EXPECT_FALSE(remove.isBusy());
	EXPECT_FALSE(remove.isRetryable());
	EXPECT_TRUE(remove.isDeleted());
}

typedef Fixture BucketVerifyTest;

TEST_F(BucketVerifyTest, Heap) {
	BucketTest * test = new BucketTest("BucketCreateTestHeap");
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isExistent());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketVerifyTest, Stack) {
	BucketTest test("BucketCreateTestStack");
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
	BucketTest test("BucketCreateTestExplicit", context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketVerifyTest, Complete) {
	Multiplex multiplex;
	BucketTest test("BucketCreateTestComplete", multiplex);
	test.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isExistent());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketVerifyTest, Service) {
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	BucketTest test("BucketCreateTestService", multiplex);
	test.start();
	int bits = 0;
	for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
		if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
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
