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
#include "com/diag/hayloft/s3/GrantGet.h"
#include "com/diag/hayloft/s3/GrantSet.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/size.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/Access.h"
#include "com/diag/hayloft/s3/Properties.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/convergence.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture GrantTest;

TEST_F(GrantTest, GetSynchronous) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetSynchronous";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket, LEVEL);
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject, LEVEL);
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, GetAsynchronous) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetAsynchronous";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	Multiplex multiplex;
	BucketCreate bucketcreate(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate, multiplex);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket, LEVEL);
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize);
	objectput.start();
	for (int ii = 0; ii < LIMIT; ++ii) {
		multiplex.complete();
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput, multiplex);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject, LEVEL);
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET, multiplex);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, GetPublicRead) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetPublicRead";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	GrantGet grantgetbucket(bucketcreate);
	ASSERT_TRUE(complete(grantgetbucket));
	show(grantgetbucket, LEVEL);
	Properties properties;
	properties.setAccess(access);
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize, properties);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput);
	ASSERT_TRUE(complete(grantgetobject));
	show(grantgetobject, LEVEL);
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	ASSERT_TRUE(complete(objectdelete));
	BucketDelete bucketdelete(BUCKET);
	ASSERT_TRUE(complete(bucketdelete));
}

TEST_F(GrantTest, GetSet) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetSet";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	/**/
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete(bucketcreate));
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	/**/
	GrantGet grantgetobject1(objectput);
	ASSERT_TRUE(complete(grantgetobject1));
	show(grantgetobject1, LEVEL);
	EXPECT_EQ(grantgetobject1.getAccessControlList().size(), 1);
	/**/
	Grant grant;
	grant.setOwnerId(grantgetobject1.getOwnerId());
	grant.setOwnerDisplayName(grantgetobject1.getOwnerDisplayName());
	grant.import(::S3GranteeTypeCanonicalUser, ::S3PermissionFullControl, grantgetobject1.getOwnerId(), grantgetobject1.getOwnerDisplayName());
	grant.import(::S3GranteeTypeAmazonCustomerByEmail, ::S3PermissionRead, "jsloan@diag.com");
	grant.import(::S3GranteeTypeAllUsers, ::S3PermissionRead);
	grant.import(::S3GranteeTypeAllAwsUsers, ::S3PermissionRead);
	show(grant, LEVEL);
	EXPECT_EQ(grant.getAccessControlList().size(), 4);
	/**/
	GrantSet grantsetobject(objectput, grant);
	EXPECT_TRUE(complete(grantsetobject));
	show(grantsetobject, LEVEL);
	/**/
	GrantGet grantgetobject2(objectput);
	EXPECT_TRUE(complete(grantgetobject2));
	show(grantgetobject2, LEVEL);
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
}

#endif
