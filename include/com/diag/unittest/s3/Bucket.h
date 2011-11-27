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
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketHead.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/ServiceManifest.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

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

TEST_F(BucketTest, Heap) {
	static const char BUCKET[] = "BucketTestHeap";
	static const int LIMIT = 10;
	{
		BucketHead * buckethead = new BucketHead(BUCKET);
		for (int ii = 0; buckethead->isRetryable() && (ii < LIMIT); ++ii) {
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			delete buckethead;
			buckethead = new BucketHead(BUCKET);
		}
		EXPECT_TRUE((*buckethead) == true);
		EXPECT_FALSE(buckethead->isIdle());
		EXPECT_FALSE(buckethead->isBusy());
		EXPECT_FALSE(buckethead->isRetryable());
		EXPECT_FALSE(buckethead->isInaccessible());
		EXPECT_TRUE(buckethead->isNonexistent());
		ASSERT_FALSE(buckethead->isSuccessful());
		delete buckethead;
	}
	{
		BucketCreate * bucketcreate = new BucketCreate(BUCKET);
		for (int ii = 0; bucketcreate->isRetryable() && (ii < LIMIT); ++ii) {
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			delete bucketcreate;
			bucketcreate = new BucketCreate(BUCKET);
		}
		EXPECT_TRUE((*bucketcreate) == true);
		EXPECT_FALSE(bucketcreate->isIdle());
		EXPECT_FALSE(bucketcreate->isBusy());
		EXPECT_FALSE(bucketcreate->isRetryable());
		EXPECT_FALSE(bucketcreate->isInaccessible());
		EXPECT_FALSE(bucketcreate->isNonexistent());
		ASSERT_TRUE(bucketcreate->isSuccessful());
		delete bucketcreate;
	}
	{
		BucketHead * buckethead = new BucketHead(BUCKET);
		for (int ii = 0; (buckethead->isRetryable() || (!buckethead->isSuccessful())) && (ii < LIMIT); ++ii) {
			if (buckethead->isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (!buckethead->isSuccessful()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			delete buckethead;
			buckethead = new BucketHead(BUCKET);
		}
		EXPECT_TRUE((*buckethead) == true);
		EXPECT_FALSE(buckethead->isIdle());
		EXPECT_FALSE(buckethead->isBusy());
		EXPECT_FALSE(buckethead->isRetryable());
		EXPECT_FALSE(buckethead->isInaccessible());
		EXPECT_FALSE(buckethead->isNonexistent());
		ASSERT_TRUE(buckethead->isSuccessful());
		delete buckethead;
	}
	{
		BucketDelete * bucketdelete = new BucketDelete(BUCKET);
		for (int ii = 0; (bucketdelete->isRetryable() || bucketdelete->isNonexistent()) && (ii < LIMIT); ++ii) {
			if (bucketdelete->isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (bucketdelete->isNonexistent()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			delete bucketdelete;
			bucketdelete = new BucketDelete(BUCKET);
		}
		EXPECT_TRUE((*bucketdelete) == true);
		EXPECT_FALSE(bucketdelete->isIdle());
		EXPECT_FALSE(bucketdelete->isBusy());
		EXPECT_FALSE(bucketdelete->isRetryable());
		EXPECT_FALSE(bucketdelete->isInaccessible());
		EXPECT_FALSE(bucketdelete->isNonexistent());
		ASSERT_TRUE(bucketdelete->isSuccessful());
		delete bucketdelete;
	}
	{
		BucketHead * buckethead = new BucketHead(BUCKET);
		for (int ii = 0; (buckethead->isRetryable() || buckethead->isSuccessful()) && (ii < LIMIT); ++ii) {
			if (buckethead->isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (buckethead->isSuccessful()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			delete buckethead;
			buckethead = new BucketHead(BUCKET);
		}
		EXPECT_TRUE((*buckethead) == true);
		EXPECT_FALSE(buckethead->isIdle());
		EXPECT_FALSE(buckethead->isBusy());
		EXPECT_FALSE(buckethead->isRetryable());
		EXPECT_FALSE(buckethead->isInaccessible());
		EXPECT_TRUE(buckethead->isNonexistent());
		ASSERT_FALSE(buckethead->isSuccessful());
		delete buckethead;
	}
}

TEST_F(BucketTest, Stack) {
	static const char BUCKET[] = "BucketTestStack";
	static const int LIMIT = 10;
	{
		BucketHead buckethead(BUCKET);
		for (int ii = 0; buckethead.isRetryable() && (ii < LIMIT); ++ii) {
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			buckethead.start();
		}
		EXPECT_TRUE(buckethead == true);
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
	{
		BucketCreate bucketcreate(BUCKET);
		for (int ii = 0; bucketcreate.isRetryable() && (ii < LIMIT); ++ii) {
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			bucketcreate.start();
		}
		EXPECT_TRUE(bucketcreate == true);
		EXPECT_FALSE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		ASSERT_TRUE(bucketcreate.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET);
		for (int ii = 0; (buckethead.isRetryable() || (!buckethead.isSuccessful())) && (ii < LIMIT); ++ii) {
			if (buckethead.isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (!buckethead.isSuccessful()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			buckethead.start();
		}
		EXPECT_TRUE(buckethead == true);
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		ASSERT_TRUE(buckethead.isSuccessful());
	}
	{
		BucketDelete bucketdelete(BUCKET);
		for (int ii = 0; (bucketdelete.isRetryable() || bucketdelete.isNonexistent()) && (ii < LIMIT); ++ii) {
			if (bucketdelete.isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (bucketdelete.isNonexistent()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			bucketdelete.start();
		}
		EXPECT_TRUE(bucketdelete == true);
		EXPECT_FALSE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		ASSERT_TRUE(bucketdelete.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET);
		for (int ii = 0; (buckethead.isRetryable() || buckethead.isSuccessful()) && (ii < LIMIT); ++ii) {
			if (buckethead.isRetryable()) {
				printf("RETRYING %d\n", __LINE__);
			} else if (buckethead.isSuccessful()) {
				printf("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			buckethead.start();
		}
		EXPECT_TRUE(buckethead == true);
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
}

TEST_F(BucketTest, Explicit) {
	static const char BUCKET[] = "BucketTestExplicit";
	static const int LIMIT = 10;
	EndpointNorthernCalifornia endpoint;
	Session session(".test.diag.com", "test.diag.com", endpoint);
	Credentials credentials(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()));
	RegionNorthernCalifornia constraint;
	ProtocolUnsecure protocol;
	StylePath style;
	AccessPublicRead access;
	Context context(credentials, constraint, protocol, style, access);
	/**/
	BucketHead buckethead(BUCKET, context, session);
	for (int ii = 0; buckethead.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		buckethead.start();
	}
	EXPECT_TRUE(buckethead == true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_TRUE(buckethead.isNonexistent());
	ASSERT_FALSE(buckethead.isSuccessful());
	/**/
	BucketCreate bucketcreate(BUCKET, context, session);
	for (int ii = 0; bucketcreate.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		bucketcreate.start();
	}
	EXPECT_TRUE(bucketcreate == true);
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	buckethead.start();
	for (int ii = 0; (buckethead.isRetryable() || (!buckethead.isSuccessful())) && (ii < LIMIT); ++ii) {
		if (buckethead.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
		} else if (!buckethead.isSuccessful()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		buckethead.start();
	}
	EXPECT_TRUE(buckethead == true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	ASSERT_TRUE(buckethead.isSuccessful());
	/**/
	ASSERT_NE(buckethead.getRegion(), (char *)0);
	ASSERT_NE(bucketcreate.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(buckethead.getRegion(), bucketcreate.getRegion()), 0);
	/**/
	BucketDelete bucketdelete(BUCKET, context, session);
	for (int ii = 0; (bucketdelete.isRetryable() || bucketdelete.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketdelete.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
		} else if (bucketdelete.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		bucketdelete.start();
	}
	EXPECT_TRUE(bucketdelete == true);
	EXPECT_FALSE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	ASSERT_TRUE(bucketdelete.isSuccessful());
	/**/
	buckethead.start();
	for (int ii = 0; (buckethead.isRetryable() || buckethead.isSuccessful()) && (ii < LIMIT); ++ii) {
		if (buckethead.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
		} else if (buckethead.isSuccessful()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		buckethead.start();
	}
	EXPECT_TRUE(buckethead == true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_TRUE(buckethead.isNonexistent());
	ASSERT_FALSE(buckethead.isSuccessful());
}

TEST_F(BucketTest, Complete) {
	static const char BUCKET[] = "BucketTestComplete";
	static const int LIMIT = 10;
	Multiplex multiplex;
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			buckethead.start();
			EXPECT_FALSE(buckethead == true);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			EXPECT_TRUE(multiplex.complete());
			EXPECT_TRUE(buckethead == true);
			if (!buckethead.isRetryable()) { break; }
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
	{
		BucketCreate bucketcreate(BUCKET, multiplex);
		EXPECT_FALSE(bucketcreate == true);
		EXPECT_TRUE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		EXPECT_FALSE(bucketcreate.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			bucketcreate.start();
			EXPECT_FALSE(bucketcreate == true);
			EXPECT_FALSE(bucketcreate.isIdle());
			EXPECT_TRUE(bucketcreate.isBusy());
			EXPECT_FALSE(bucketcreate.isRetryable());
			EXPECT_FALSE(bucketcreate.isInaccessible());
			EXPECT_FALSE(bucketcreate.isNonexistent());
			EXPECT_FALSE(bucketcreate.isSuccessful());
			EXPECT_TRUE(multiplex.complete());
			EXPECT_TRUE(bucketcreate == true);
			if (!bucketcreate.isRetryable()) { break; }
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		ASSERT_TRUE(bucketcreate.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				buckethead.start();
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				EXPECT_TRUE(multiplex.complete());
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (buckethead.isSuccessful()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		ASSERT_TRUE(buckethead.isSuccessful());
	}
	{
		BucketDelete bucketdelete(BUCKET, multiplex);
		EXPECT_FALSE(bucketdelete == true);
		EXPECT_TRUE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		EXPECT_FALSE(bucketdelete.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				bucketdelete.start();
				EXPECT_FALSE(bucketdelete == true);
				EXPECT_FALSE(bucketdelete.isIdle());
				EXPECT_TRUE(bucketdelete.isBusy());
				EXPECT_FALSE(bucketdelete.isRetryable());
				EXPECT_FALSE(bucketdelete.isInaccessible());
				EXPECT_FALSE(bucketdelete.isNonexistent());
				EXPECT_FALSE(bucketdelete.isSuccessful());
				EXPECT_TRUE(multiplex.complete());
				EXPECT_TRUE(bucketdelete == true);
				if (!bucketdelete.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!bucketdelete.isNonexistent()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		ASSERT_TRUE(bucketdelete.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				buckethead.start();
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				EXPECT_TRUE(multiplex.complete());
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!buckethead.isSuccessful()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
}

TEST_F(BucketTest, Service) {
	static const char BUCKET[] = "BucketTestService";
	static const int LIMIT = 10;
	static const Multiplex::Milliseconds TIMEOUT = 1000;
	Multiplex multiplex;
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			buckethead.start();
			EXPECT_FALSE(buckethead == true);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			int bits = 0;
			for (int ll = 0; (buckethead != true) && (ll < LIMIT); ++ll) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				printf("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(buckethead == true);
			if (!buckethead.isRetryable()) { break; }
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
	{
		BucketCreate bucketcreate(BUCKET, multiplex);
		EXPECT_FALSE(bucketcreate == true);
		EXPECT_TRUE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		EXPECT_FALSE(bucketcreate.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			bucketcreate.start();
			EXPECT_FALSE(bucketcreate == true);
			EXPECT_FALSE(bucketcreate.isIdle());
			EXPECT_TRUE(bucketcreate.isBusy());
			EXPECT_FALSE(bucketcreate.isRetryable());
			EXPECT_FALSE(bucketcreate.isInaccessible());
			EXPECT_FALSE(bucketcreate.isNonexistent());
			EXPECT_FALSE(bucketcreate.isSuccessful());
			int bits = 0;
			for (int ll = 0; (bucketcreate != true) && (ll < LIMIT); ++ll) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				printf("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(bucketcreate == true);
			if (!bucketcreate.isRetryable()) { break; }
			printf("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		ASSERT_TRUE(bucketcreate.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				buckethead.start();
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				int bits = 0;
				for (int ll = 0; (buckethead != true) && (ll < LIMIT); ++ll) {
					if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
					printf("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (buckethead.isSuccessful()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		ASSERT_TRUE(buckethead.isSuccessful());
	}
	{
		BucketDelete bucketdelete(BUCKET, multiplex);
		EXPECT_FALSE(bucketdelete == true);
		EXPECT_TRUE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		EXPECT_FALSE(bucketdelete.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				bucketdelete.start();
				EXPECT_FALSE(bucketdelete == true);
				EXPECT_FALSE(bucketdelete.isIdle());
				EXPECT_TRUE(bucketdelete.isBusy());
				EXPECT_FALSE(bucketdelete.isRetryable());
				EXPECT_FALSE(bucketdelete.isInaccessible());
				EXPECT_FALSE(bucketdelete.isNonexistent());
				EXPECT_FALSE(bucketdelete.isSuccessful());
				int bits = 0;
				for (int ll = 0; (bucketdelete != true) && (ll < LIMIT); ++ll) {
					if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
					printf("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(bucketdelete == true);
				if (!bucketdelete.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!bucketdelete.isNonexistent()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		ASSERT_TRUE(bucketdelete.isSuccessful());
	}
	{
		BucketHead buckethead(BUCKET, multiplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				buckethead.start();
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				int bits = 0;
				for (int ll = 0; (buckethead != true) && (ll < LIMIT); ++ll) {
					if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
					printf("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				printf("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!buckethead.isSuccessful()) { break; }
			printf("WAITING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_TRUE(buckethead.isNonexistent());
		ASSERT_FALSE(buckethead.isSuccessful());
	}
}

TEST_F(BucketTest, List) {
	static const int LIMIT = 10;
	const char BUCKET1[] = "BucketTestList1";
	const char BUCKET2[] = "BucketTestList2";
	ServiceManifest servicemanifest1;
	for (int ii = 0; servicemanifest1.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		servicemanifest1.start();
	}
	EXPECT_EQ(servicemanifest1.getList().size(), 0);
	BucketCreate bucketcreate1(BUCKET1);
	for (int ii = 0; bucketcreate1.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		bucketcreate1.start();
	}
	ServiceManifest servicemanifest2;
	for (int ii = 0; servicemanifest2.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		servicemanifest2.start();
	}
	EXPECT_EQ(servicemanifest2.getList().size(), 1);
	ASSERT_NE(servicemanifest2.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	BucketCreate bucketcreate2(BUCKET2);
	for (int ii = 0; bucketcreate2.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		bucketcreate2.start();
	}
	ServiceManifest servicemanifest3;
	for (int ii = 0; servicemanifest3.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		servicemanifest3.start();
	}
	EXPECT_EQ(servicemanifest3.getList().size(), 2);
	ASSERT_NE(servicemanifest3.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	ASSERT_NE(servicemanifest3.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
	BucketDelete bucketdelete1(BUCKET1);
	for (int ii = 0; (bucketdelete1.isRetryable() || bucketdelete1.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketdelete1.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
		} else if (bucketdelete1.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		bucketdelete1.start();
	}
	ServiceManifest servicemanifest4;
	for (int ii = 0; servicemanifest4.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		servicemanifest4.start();
	}
	EXPECT_EQ(servicemanifest4.getList().size(), 1);
	EXPECT_EQ(servicemanifest4.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	ASSERT_NE(servicemanifest4.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
	BucketDelete bucketdelete2(BUCKET2);
	for (int ii = 0; (bucketdelete2.isRetryable() || bucketdelete2.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketdelete2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
		} else if (bucketdelete2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		bucketdelete2.start();
	}
	ServiceManifest servicemanifest5;
	for (int ii = 0; servicemanifest5.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		servicemanifest5.start();
	}
	EXPECT_EQ(servicemanifest5.getList().size(), 0);
	EXPECT_EQ(servicemanifest5.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	EXPECT_EQ(servicemanifest5.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
}

}
}
}
}

#endif
