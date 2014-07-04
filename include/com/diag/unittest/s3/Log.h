/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_GRANT_
#define _H__COM_DIAG_UNITTEST_S3_GRANT_

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
#include "com/diag/hayloft/s3/LogGet.h"
#include "com/diag/hayloft/s3/LogSet.h"
#include "com/diag/hayloft/s3/GrantGet.h"
#include "com/diag/hayloft/s3/GrantSet.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/convergence.h"
#include "com/diag/hayloft/Fibonacci.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture LogTest;

TEST_F(LogTest, GetSynchronous) {
	static const char BUCKET[] = "LogTestGetSynchronous";
	BucketCreate bucket(BUCKET);
	ASSERT_TRUE(complete(bucket));
	LogGet logget(bucket);
	ASSERT_TRUE(complete(logget));
	ASSERT_NE(logget.getTarget(), (char *)0);
	EXPECT_EQ(*logget.getTarget(), '\0');
	ASSERT_NE(logget.getPrefix(), (char *)0);
	EXPECT_EQ(*logget.getPrefix(), '\0');
	show(logget);
	BucketDelete bucketdelete(bucket);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(LogTest, GetAsynchronous) {
	static const char BUCKET[] = "LogTestGetAsynchronous";
	Multiplex multiplex;
	BucketCreate bucket(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucket));
	LogGet logget(bucket, multiplex);
	ASSERT_TRUE(complete(logget));
	ASSERT_NE(logget.getTarget(), (char *)0);
	EXPECT_EQ(*logget.getTarget(), '\0');
	ASSERT_NE(logget.getPrefix(), (char *)0);
	EXPECT_EQ(*logget.getPrefix(), '\0');
	show(logget);
	BucketDelete bucketdelete(bucket, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(LogTest, SetGet) {
	static const char BUCKET[] = "LogTestSetGetBucket";
	static const char LOG[] = "LogTestSetGetLog";
	Multiplex multiplex;
	BucketCreate bucket(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucket));
	LogGet logget1(bucket, multiplex);
	ASSERT_TRUE(complete(logget1));
	ASSERT_NE(logget1.getTarget(), (char *)0);
	EXPECT_EQ(*logget1.getTarget(), '\0');
	ASSERT_NE(logget1.getPrefix(), (char *)0);
	EXPECT_EQ(*logget1.getPrefix(), '\0');
	show(logget1);
	GrantGet grantget(bucket, multiplex);
	ASSERT_TRUE(complete(grantget));
	show(grantget);
	BucketCreate log(LOG, multiplex);
	ASSERT_TRUE(complete(log));
	GrantSet grant(log, multiplex, grantget);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionWrite);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionReadACP);
	show(grant);
	ASSERT_TRUE(complete(grant));
	LogSet logset(bucket, multiplex, log);
	ASSERT_NE(logset.getTarget(), (char *)0);
	EXPECT_EQ(std::strcmp(logset.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(logset.getPrefix(), (char *)0);
	EXPECT_EQ(*logset.getPrefix(), '\0');
	ASSERT_TRUE(complete(logset));
	Fibonacci fibonacci;
	LogGet logget2(bucket, multiplex);
	for (int ii = 0; ii < 10; ++ii) {
		ASSERT_TRUE(complete(logget2));
		ASSERT_NE(logget2.getTarget(), (char *)0);
		if (std::strcmp(logget2.getTarget(), log.getCanonical()) == 0) { break; }
		::com::diag::desperado::Platform::instance().yield(fibonacci * ::com::diag::desperado::Platform::instance().frequency());
		EXPECT_TRUE(logget2.start());
	}
	EXPECT_EQ(std::strcmp(logget2.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(logget2.getPrefix(), (char *)0);
	EXPECT_EQ(*logget2.getPrefix(), '\0');
	show(logget2);
	BucketDelete bucketdelete(bucket, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
	BucketDelete logdelete(log, multiplex);
	ASSERT_TRUE(complete(logdelete));
}

TEST_F(LogTest, SetGetPrefix) {
	static const char BUCKET[] = "LogTestSetGetPrefixBucket";
	static const char LOG[] = "LogTestSetGetPrefixLog";
	static const char PREFIX[] = "Prefix";
	Multiplex multiplex;
	BucketCreate bucket(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucket));
	LogGet logget1(bucket, multiplex);
	ASSERT_TRUE(complete(logget1));
	ASSERT_NE(logget1.getTarget(), (char *)0);
	EXPECT_EQ(*logget1.getTarget(), '\0');
	ASSERT_NE(logget1.getPrefix(), (char *)0);
	EXPECT_EQ(*logget1.getPrefix(), '\0');
	show(logget1);
	GrantGet grantget(bucket, multiplex);
	ASSERT_TRUE(complete(grantget));
	show(grantget);
	BucketCreate log(LOG, multiplex);
	ASSERT_TRUE(complete(log));
	GrantSet grant(log, multiplex, grantget);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionWrite);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionReadACP);
	show(grant);
	ASSERT_TRUE(complete(grant));
	LogSet logset(bucket, multiplex, log, PREFIX);
	ASSERT_NE(logset.getTarget(), (char *)0);
	EXPECT_EQ(std::strcmp(logset.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(logset.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(logset.getPrefix(), PREFIX), 0);
	ASSERT_TRUE(complete(logset));
	Fibonacci fibonacci;
	LogGet logget2(bucket, multiplex);
	for (int ii = 0; ii < 10; ++ii) {
		ASSERT_TRUE(complete(logget2));
		ASSERT_NE(logget2.getTarget(), (char *)0);
		if (std::strcmp(logget2.getTarget(), log.getCanonical()) == 0) { break; }
		::com::diag::desperado::Platform::instance().yield(fibonacci * ::com::diag::desperado::Platform::instance().frequency());
		EXPECT_TRUE(logget2.start());
	}
	EXPECT_EQ(std::strcmp(logget2.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(logget2.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(logget2.getPrefix(), PREFIX), 0);
	show(logget2);
	BucketDelete bucketdelete(bucket, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
	BucketDelete logdelete(log, multiplex);
	ASSERT_TRUE(complete(logdelete));
}

// There's no good way I know of to functionally test this within this suite,
// since S3 will write the logs asynchronously at some point in the future.
// I pretend to test it here, but it would be a miracle if this unit test just
// happen to run as S3 is writing the logs so that the second Bucket Manifest
// Action saw them.
TEST_F(LogTest, SetGetApplication) {
	static const char BUCKET[] = "LogTestSetGetApplicationBucket";
	static const char LOG[] = "LogTestSetGetApplicationLog";
	static const char PREFIX[] = "Prefix";
	static const char OBJECT[] = "Object.txt";
	Multiplex multiplex;
	BucketCreate bucket(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucket));
	GrantGet grantget(bucket, multiplex);
	ASSERT_TRUE(complete(grantget));
	BucketCreate log(LOG, multiplex);
	ASSERT_TRUE(complete(log));
	GrantSet grant(log, multiplex, grantget);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionWrite);
	grant.import(::S3GranteeTypeLogDelivery, ::S3PermissionReadACP);
	ASSERT_TRUE(complete(grant));
	LogSet logset(bucket, multiplex, log, PREFIX);
	ASSERT_TRUE(complete(logset));
	Fibonacci factor;
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut put(OBJECT, bucket, multiplex, input, inputsize);
	for (int ii = 0; ii < 10; ++ii) {
		EXPECT_TRUE(put.start());
		multiplex.complete();
		if (!put.isRetryable()) { break; }
		platform.yield(factor * platform.frequency());
		input = new ::com::diag::desperado::PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(put.reset(input, inputsize));
	}
	ASSERT_TRUE(put.isSuccessful());
	BucketManifest manifest1(bucket, multiplex);
	ASSERT_TRUE(complete(manifest1));
	show(manifest1);
	ObjectDelete objectdelete(put, multiplex);
	ASSERT_TRUE(complete(objectdelete));
	BucketManifest manifest2(log, multiplex);
	ASSERT_TRUE(complete(manifest2));
	show(manifest2);
	BucketDelete bucketdelete(bucket, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
	BucketDelete logdelete(log, multiplex);
	ASSERT_TRUE(complete(logdelete));
}

}
}
}
}

#endif
