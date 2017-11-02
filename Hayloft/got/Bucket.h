/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_BUCKET_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_BUCKET_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/BucketHead.h"
#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/hayloft/ServiceManifest.h"
#include "com/diag/hayloft/Multiplex.h"
#include "com/diag/hayloft/Simplex.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

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
			logger.configuration("RETRYING %d\n", __LINE__);
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
			logger.configuration("RETRYING %d\n", __LINE__);
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (!buckethead->isSuccessful()) {
				logger.configuration("WAITING %d\n", __LINE__);
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (bucketdelete->isNonexistent()) {
				logger.configuration("WAITING %d\n", __LINE__);
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (buckethead->isSuccessful()) {
				logger.configuration("WAITING %d\n", __LINE__);
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
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			EXPECT_TRUE(buckethead.start());
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
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			EXPECT_TRUE(bucketcreate.start());
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (!buckethead.isSuccessful()) {
				logger.configuration("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			EXPECT_TRUE(buckethead.start());
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (bucketdelete.isNonexistent()) {
				logger.configuration("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			EXPECT_TRUE(bucketdelete.start());
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
				logger.configuration("RETRYING %d\n", __LINE__);
			} else if (buckethead.isSuccessful()) {
				logger.configuration("WAITING %d\n", __LINE__);
			}
			platform.yield(platform.frequency());
			EXPECT_TRUE(buckethead.start());
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
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(buckethead.start());
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
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate.start());
	}
	EXPECT_TRUE(bucketcreate == true);
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	EXPECT_TRUE(buckethead.start());
	for (int ii = 0; (buckethead.isRetryable() || (!buckethead.isSuccessful())) && (ii < LIMIT); ++ii) {
		if (buckethead.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (!buckethead.isSuccessful()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(buckethead.start());
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
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketdelete.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketdelete.start());
	}
	EXPECT_TRUE(bucketdelete == true);
	EXPECT_FALSE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	ASSERT_TRUE(bucketdelete.isSuccessful());
	/**/
	EXPECT_TRUE(buckethead.start());
	for (int ii = 0; (buckethead.isRetryable() || buckethead.isSuccessful()) && (ii < LIMIT); ++ii) {
		if (buckethead.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (buckethead.isSuccessful()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(buckethead.start());
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
			EXPECT_TRUE(buckethead.start());
			EXPECT_FALSE(buckethead == true);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_TRUE(buckethead == true);
			if (!buckethead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
			EXPECT_TRUE(bucketcreate.start());
			EXPECT_FALSE(bucketcreate == true);
			EXPECT_FALSE(bucketcreate.isIdle());
			EXPECT_TRUE(bucketcreate.isBusy());
			EXPECT_FALSE(bucketcreate.isRetryable());
			EXPECT_FALSE(bucketcreate.isInaccessible());
			EXPECT_FALSE(bucketcreate.isNonexistent());
			EXPECT_FALSE(bucketcreate.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_TRUE(bucketcreate == true);
			if (!bucketcreate.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
				EXPECT_TRUE(buckethead.start());
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				EXPECT_EQ(multiplex.complete(), 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
				EXPECT_TRUE(bucketdelete.start());
				EXPECT_FALSE(bucketdelete == true);
				EXPECT_FALSE(bucketdelete.isIdle());
				EXPECT_TRUE(bucketdelete.isBusy());
				EXPECT_FALSE(bucketdelete.isRetryable());
				EXPECT_FALSE(bucketdelete.isInaccessible());
				EXPECT_FALSE(bucketdelete.isNonexistent());
				EXPECT_FALSE(bucketdelete.isSuccessful());
				EXPECT_EQ(multiplex.complete(), 0);
				EXPECT_TRUE(bucketdelete == true);
				if (!bucketdelete.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!bucketdelete.isNonexistent()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
				EXPECT_TRUE(buckethead.start());
				EXPECT_FALSE(buckethead == true);
				EXPECT_FALSE(buckethead.isIdle());
				EXPECT_TRUE(buckethead.isBusy());
				EXPECT_FALSE(buckethead.isRetryable());
				EXPECT_FALSE(buckethead.isInaccessible());
				EXPECT_FALSE(buckethead.isNonexistent());
				EXPECT_FALSE(buckethead.isSuccessful());
				EXPECT_EQ(multiplex.complete(), 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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

TEST_F(BucketTest, Simplex) {
	static const char BUCKET[] = "BucketTestSimplex";
	static const int LIMIT = 10;
	Simplex simplex;
	{
		BucketHead buckethead(BUCKET, simplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_TRUE(buckethead == true);
			if (!buckethead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
		BucketCreate bucketcreate(BUCKET, simplex);
		EXPECT_FALSE(bucketcreate == true);
		EXPECT_TRUE(bucketcreate.isIdle());
		EXPECT_FALSE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		EXPECT_FALSE(bucketcreate.isSuccessful());
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(bucketcreate.start());
			EXPECT_TRUE(bucketcreate == true);
			if (!bucketcreate.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
		BucketHead buckethead(BUCKET, simplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				EXPECT_TRUE(buckethead.start());
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
		BucketDelete bucketdelete(BUCKET, simplex);
		EXPECT_FALSE(bucketdelete == true);
		EXPECT_TRUE(bucketdelete.isIdle());
		EXPECT_FALSE(bucketdelete.isBusy());
		EXPECT_FALSE(bucketdelete.isRetryable());
		EXPECT_FALSE(bucketdelete.isInaccessible());
		EXPECT_FALSE(bucketdelete.isNonexistent());
		EXPECT_FALSE(bucketdelete.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				EXPECT_TRUE(bucketdelete.start());
				EXPECT_TRUE(bucketdelete == true);
				if (!bucketdelete.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!bucketdelete.isNonexistent()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
		BucketHead buckethead(BUCKET, simplex);
		EXPECT_FALSE(buckethead == true);
		EXPECT_TRUE(buckethead.isIdle());
		EXPECT_FALSE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		for (int jj = 0; jj < LIMIT; ++jj) {
			for (int ii = 0; ii < LIMIT; ++ii) {
				EXPECT_TRUE(buckethead.start());
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
	static const int LIMIT = 60;
	static const Milliseconds TIMEOUT = 1000;
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
			EXPECT_TRUE(buckethead.start());
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
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(buckethead == true);
			if (!buckethead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
			EXPECT_TRUE(bucketcreate.start());
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
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_TRUE(bucketcreate == true);
			if (!bucketcreate.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
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
				EXPECT_TRUE(buckethead.start());
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
					logger.configuration("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
				EXPECT_TRUE(bucketdelete.start());
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
					logger.configuration("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(bucketdelete == true);
				if (!bucketdelete.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!bucketdelete.isNonexistent()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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
				EXPECT_TRUE(buckethead.start());
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
					logger.configuration("TIMEDOUT %d\n", __LINE__);
				}
				EXPECT_EQ(bits, 0);
				EXPECT_TRUE(buckethead == true);
				if (!buckethead.isRetryable()) { break; }
				logger.configuration("RETRYING %d\n", __LINE__);
				platform.yield(platform.frequency());
			}
			if (!buckethead.isSuccessful()) { break; }
			logger.configuration("WAITING %d\n", __LINE__);
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

TEST_F(BucketTest, Manifest) {
	static const int LIMIT = 10;
	const char BUCKET1[] = "BucketTestManifest1";
	const char BUCKET2[] = "BucketTestManifest2";
	ServiceManifest servicemanifest1;
	for (int ii = 0; servicemanifest1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(servicemanifest1.start());
	}
	EXPECT_EQ(servicemanifest1.getManifest().size(), 0);
	BucketCreate bucketcreate1(BUCKET1);
	for (int ii = 0; bucketcreate1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate1.start());
	}
	ServiceManifest servicemanifest2;
	for (int ii = 0; servicemanifest2.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(servicemanifest2.start());
	}
	EXPECT_EQ(servicemanifest2.getManifest().size(), 1);
	EXPECT_NE(servicemanifest2.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	BucketCreate bucketcreate2(BUCKET2);
	for (int ii = 0; bucketcreate2.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate2.start());
	}
	ServiceManifest servicemanifest3;
	for (int ii = 0; servicemanifest3.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(servicemanifest3.start());
	}
	EXPECT_EQ(servicemanifest3.getManifest().size(), 2);
	EXPECT_NE(servicemanifest3.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	EXPECT_NE(servicemanifest3.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
	show(servicemanifest3);
	EXPECT_TRUE(servicemanifest3.reset());
	EXPECT_EQ(servicemanifest3.getManifest().size(), 0);
	EXPECT_EQ(servicemanifest3.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	EXPECT_EQ(servicemanifest3.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
	BucketDelete bucketdelete1(BUCKET1);
	for (int ii = 0; (bucketdelete1.isRetryable() || bucketdelete1.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketdelete1.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketdelete1.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketdelete1.start());
	}
	ServiceManifest servicemanifest4;
	for (int ii = 0; servicemanifest4.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(servicemanifest4.start());
	}
	EXPECT_EQ(servicemanifest4.getManifest().size(), 1);
	EXPECT_EQ(servicemanifest4.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	EXPECT_NE(servicemanifest4.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
	BucketDelete bucketdelete2(BUCKET2);
	for (int ii = 0; (bucketdelete2.isRetryable() || bucketdelete2.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketdelete2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketdelete2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketdelete2.start());
	}
	ServiceManifest servicemanifest5;
	for (int ii = 0; servicemanifest5.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(servicemanifest5.start());
	}
	EXPECT_EQ(servicemanifest5.getManifest().size(), 0);
	EXPECT_EQ(servicemanifest5.find(bucketcreate1.getCanonical()), (ServiceManifest::Entry *)0);
	EXPECT_EQ(servicemanifest5.find(bucketcreate2.getCanonical()), (ServiceManifest::Entry *)0);
}

}
}
}

#endif
