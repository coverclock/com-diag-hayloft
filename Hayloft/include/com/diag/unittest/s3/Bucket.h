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
	EXPECT_FALSE(bucket->isIdle());
	EXPECT_FALSE(bucket->isBusy());
	EXPECT_FALSE(bucket->isRetryable());
	EXPECT_EQ(bucket->getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket->getName(), (char *)0);
	delete bucket;
}

TEST_F(BucketBaseTest, Stack) {
	Bucket bucket("BucketBaseTestStack");
	EXPECT_TRUE(bucket == true);
	EXPECT_FALSE(bucket.isIdle());
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
	EXPECT_TRUE(valid->isSuccessful());
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	BucketValid valid("BucketValidTestStack");
	EXPECT_TRUE(valid == true);
	EXPECT_TRUE(valid.isSuccessful());
}

// In the unit tests below, we are testing against the actual AWS S3, not a
// mock. We see two types of failures: RETRYING means it was a recoverable
// failure, most typically "Failed To Connect"; WAITING means the unit test
// detected that the result of the prior operation has not been propagated
// through the S3 distributed server network, a result of its Eventual
// Consistency architecture. Most applications will need to address the former,
// unless they are using a "Best Effort" model. Few applications will need to
// address the latter, since the system becomes consistent very quickly and a
// small amount of lag isn't likely to be an issue. TIMEDOUT merely means that
// the socket select(2) timed out waiting for sockets to become ready for
// reading. In a real application that just means it would have time to go do
// other work (and other threads would run while the thread was waiting on
// the select(2) to return).

typedef Fixture BucketTest;

#if 0
TEST_F(BucketTest, Heap) {
	static const char BUCKET[] = "BucketTestHeap";
	BucketTest * test = new BucketTest(BUCKET);
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isIdle());
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isSuccessful());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
	BucketCreate * test = new BucketCreate(BUCKET);
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isIdle());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isSuccessful());
	delete test;
	BucketTest * test = new BucketTest(BUCKET);
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isIdle());
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_TRUE(test->isSuccessful());
	EXPECT_FALSE(test->isNonexistent());
	delete test;
	BucketDelete * test = new BucketDelete(BUCKET);
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isIdle());
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_TRUE(test->isSuccessful());
	delete test;
	BucketTest * test = new BucketTest(BUCKET);
	EXPECT_TRUE((*test) == true);
	EXPECT_FALSE(test->isIdle());
	EXPECT_FALSE(test->isBusy());
	EXPECT_FALSE(test->isRetryable());
	EXPECT_FALSE(test->isInaccessible());
	EXPECT_FALSE(test->isSuccessful());
	EXPECT_TRUE(test->isNonexistent());
	delete test;
}

TEST_F(BucketTest, Stack) {
	static const char BUCKET[] = "BucketTestStack";
	BucketTest test(BUCKET);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
	BucketCreate test(BUCKET);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isSuccessful());
	BucketTest test(BUCKET);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isSuccessful());
	EXPECT_FALSE(test.isNonexistent());
	BucketDelete test(BUCKET);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isSuccessful());
	BucketTest test(BUCKET);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketTest, Explicit) {
	static const char BUCKET[] = "BucketTestExplicit";
	Session session;
	Credentials credentials;
	Region constraint;
	Protocol protocol;
	Style style;
	Access list;
	Context context(credentials, constraint, protocol, style, list);
	BucketTest test(BUCKET, context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
	BucketCreate test(BUCKET, context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isSuccessful());
	BucketTest test(BUCKET, context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isSuccessful());
	EXPECT_FALSE(test.isNonexistent());
	BucketDelete test(BUCKET, context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_TRUE(test.isSuccessful());
	BucketTest test(BUCKET, context, session);
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
}

TEST_F(BucketTest, Complete) {
	static const char BUCKET[] = "BucketTestComplete";
	Multiplex multiplex;
	BucketTest test(BUCKET, multiplex);
	EXPECT_FALSE(test == true);
	EXPECT_TRUE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	test.start();
	EXPECT_FALSE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_TRUE(test.isBusy());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
	BucketCreate create(BUCKET, multiplex);
	create.start();
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(create == true);
	EXPECT_FALSE(create.isIdle());
	EXPECT_FALSE(create.isBusy());
	EXPECT_FALSE(create.isRetryable());
	EXPECT_TRUE(create.isSuccessful());
	BucketTest test(BUCKET, multiplex);
	EXPECT_FALSE(test == true);
	EXPECT_TRUE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	test.start();
	EXPECT_FALSE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_TRUE(test.isBusy());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_TRUE(test.isSuccessful());
	EXPECT_FALSE(test.isNonexistent());
	BucketDelete remove(BUCKET, multiplex);
	EXPECT_FALSE(remove == true);
	EXPECT_TRUE(remove.isIdle());
	EXPECT_FALSE(remove.isBusy());
	remove.start();
	EXPECT_FALSE(remove == true);
	EXPECT_FALSE(remove.isIdle());
	EXPECT_TRUE(remove.isBusy());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(remove == true);
	EXPECT_FALSE(remove.isIdle());
	EXPECT_FALSE(remove.isBusy());
	EXPECT_FALSE(remove.isRetryable());
	EXPECT_TRUE(remove.isDeleted());
	BucketTest test(BUCKET, multiplex);
	EXPECT_FALSE(test == true);
	EXPECT_TRUE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	test.start();
	EXPECT_FALSE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_TRUE(test.isBusy());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_TRUE(test == true);
	EXPECT_FALSE(test.isIdle());
	EXPECT_FALSE(test.isBusy());
	EXPECT_FALSE(test.isRetryable());
	EXPECT_FALSE(test.isInaccessible());
	EXPECT_FALSE(test.isSuccessful());
	EXPECT_TRUE(test.isNonexistent());
}
#endif

TEST_F(BucketTest, Service) {
	static const char BUCKET[] = "BucketTestService";
	static const int LIMIT = 100;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	{
		BucketTest test(BUCKET, multiplex);
		EXPECT_FALSE(test == true);
		EXPECT_TRUE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		for (int ii = 0; ii < LIMIT; ++ii) {
			test.start();
			EXPECT_FALSE(test == true);
			EXPECT_FALSE(test.isIdle());
			EXPECT_TRUE(test.isBusy());
			int bits = 0;
			for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				printf("TIMEDOUT\n");
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(test == true);
			if (!test.isRetryable()) { break; }
			printf("RETRYING\n");
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		EXPECT_FALSE(test.isRetryable());
		EXPECT_FALSE(test.isInaccessible());
		EXPECT_FALSE(test.isSuccessful());
		EXPECT_TRUE(test.isNonexistent());
	}
	{
		BucketCreate create(BUCKET, multiplex);
		EXPECT_FALSE(create == true);
		EXPECT_TRUE(create.isIdle());
		EXPECT_FALSE(create.isBusy());
		for (ii = 0; ii < LIMIT; ++ii) {
			create.start();
			EXPECT_FALSE(create == true);
			EXPECT_FALSE(create.isIdle());
			EXPECT_TRUE(create.isBusy());
			int bits = 0;
			for (int limit = LIMIT; (create != true) && (limit > 0); --limit) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				printf("TIMEDOUT\n");
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(create == true);
			if (!create.isRetryable()) { break; }
			printf("RETRYING\n");
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(create.isIdle());
		EXPECT_FALSE(create.isBusy());
		EXPECT_FALSE(create.isRetryable());
		EXPECT_TRUE(create.isSuccessful());
	}
	{
		BucketTest test(BUCKET, multiplex);
		EXPECT_FALSE(test == true);
		EXPECT_TRUE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		for (jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				test.start();
				EXPECT_FALSE(test == true);
				EXPECT_FALSE(test.isIdle());
				EXPECT_TRUE(test.isBusy());
				int bits = 0;
				for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
					if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
					printf("TIMEDOUT\n");
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(test == true);
				if (!test.isRetryable()) { break; }
				printf("RETRYING\n");
				platform.yield(platform.frequency());
			}
			if (test.isExistent()) { break; }
			printf("WAITING\n");
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		EXPECT_FALSE(test.isRetryable());
		EXPECT_FALSE(test.isInaccessible());
		EXPECT_TRUE(test.isSuccessful());
		EXPECT_FALSE(test.isNonexistent());
	}
	{
		BucketDelete remove(BUCKET, multiplex);
		EXPECT_FALSE(remove == true);
		EXPECT_TRUE(remove.isIdle());
		EXPECT_FALSE(remove.isBusy());
		for (int ii = 0; ii < LIMIT; ++ii) {
			remove.start();
			EXPECT_FALSE(remove == true);
			EXPECT_FALSE(remove.isIdle());
			EXPECT_TRUE(remove.isBusy());
			int bits = 0;
			for (int limit = LIMIT; (remove != true) && (limit > 0); --limit) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				printf("TIMEDOUT\n");
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(remove == true);
			if (!remove.isRetryable()) { break; }
			printf("RETRYING\n");
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(remove.isIdle());
		EXPECT_FALSE(remove.isBusy());
		EXPECT_FALSE(remove.isRetryable());
		EXPECT_TRUE(remove.isSuccessful());
	}
	{
		BucketTest test(BUCKET, multiplex);
		EXPECT_FALSE(test == true);
		EXPECT_TRUE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		for (jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				test.start();
				EXPECT_FALSE(test == true);
				EXPECT_FALSE(test.isIdle());
				EXPECT_TRUE(test.isBusy());
				int bits = 0;
				for (int limit = LIMIT; (test != true) && (limit > 0); --limit) {
					if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
					printf("TIMEDOUT\n");
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(test == true);
				if (!test.isRetryable()) { break; }
				printf("RETRYING\n");
				platform.yield(platform.frequency());
			}
			if (!test.isExistent()) { break; }
			printf("WAITING\n");
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(test.isIdle());
		EXPECT_FALSE(test.isBusy());
		EXPECT_FALSE(test.isRetryable());
		EXPECT_FALSE(test.isInaccessible());
		EXPECT_FALSE(test.isSuccessful());
		EXPECT_TRUE(test.isNonexistent());
	}
}

}
}
}
}

#endif
