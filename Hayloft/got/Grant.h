/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_GRANT_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_GRANT_

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
#include "com/diag/hayloft/GrantGet.h"
#include "com/diag/hayloft/GrantSet.h"
#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/grandote/size.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/Access.h"
#include "com/diag/hayloft/Properties.h"
#include "com/diag/hayloft/Context.h"
#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/convergence.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture GrantTest;

TEST_F(GrantTest, GetSynchronous) {
	static const int LIMIT = Fixture::limit(10);
	static const char BUCKET[] = "GrantTestGetSynchronous";
	static const char OBJECT[] = "Object.txt";
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket);
	PathInput * input = new PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput.reset(input, inputsize));
		EXPECT_TRUE(objectput.start());
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject);
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, GetAsynchronous) {
	static const int LIMIT = Fixture::limit(10);
	static const char BUCKET[] = "GrantTestGetAsynchronous";
	static const char OBJECT[] = "Object.txt";
	Multiplex multiplex;
	BucketCreate bucketcreate(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate, multiplex);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket);
	PathInput * input = new PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize);
	EXPECT_TRUE(objectput.start());
	for (int ii = 0; ii < LIMIT; ++ii) {
		multiplex.complete();
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput.reset(input, inputsize));
		EXPECT_TRUE(objectput.start());
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput, multiplex);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject);
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, GetPublicRead) {
	static const int LIMIT = Fixture::limit(10);
	static const char BUCKET[] = "GrantTestGetPublicRead";
	static const char OBJECT[] = "Object.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket);
	Properties properties;
	properties.setAccess(access);
	PathInput * input = new PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize, properties);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput.reset(input, inputsize));
		EXPECT_TRUE(objectput.start());
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject);
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, SetGet) {
	static const int LIMIT = Fixture::limit(10);
	static const char BUCKET[] = "GrantTestSetGet";
	static const char OBJECT[] = "Object.txt";
	/**/
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	/**/
	PathInput * input = new PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput.reset(input, inputsize));
		EXPECT_TRUE(objectput.start());
	}
	ASSERT_TRUE(objectput.isSuccessful());
	/**/
	GrantGet grantgetobject1(objectput);
	ASSERT_TRUE(complete(grantgetobject1));
	show(grantgetobject1);
	EXPECT_EQ(grantgetobject1.getAccessControlList().size(), 1);
	/**/
	Grant grant;
	grant.setOwnerId(grantgetobject1.getOwnerId());
	grant.setOwnerDisplayName(grantgetobject1.getOwnerDisplayName());
	grant.import(::S3GranteeTypeCanonicalUser, ::S3PermissionFullControl, grantgetobject1.getOwnerId(), grantgetobject1.getOwnerDisplayName());
	grant.import(::S3GranteeTypeAmazonCustomerByEmail, ::S3PermissionRead, "jsloan@diag.com");
	grant.import(::S3GranteeTypeAllUsers, ::S3PermissionRead);
	grant.import(::S3GranteeTypeAllAwsUsers, ::S3PermissionRead);
	show(grant);
	EXPECT_EQ(grant.getAccessControlList().size(), 4);
	/**/
	GrantSet grantsetobject(objectput, grant);
	EXPECT_TRUE(complete(grantsetobject));
	show(grantsetobject);
	/**/
	GrantGet grantgetobject2(objectput);
	EXPECT_TRUE(complete(grantgetobject2));
	show(grantgetobject2);
	EXPECT_EQ(grantgetobject2.getAccessControlList().size(), 4);
	/**/
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	EXPECT_TRUE(complete(objectdelete));
	/**/
	BucketDelete bucketdelete(BUCKET);
	EXPECT_TRUE(complete(bucketdelete));
}

}
}
}

#endif
