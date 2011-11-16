/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_OBJECT
#define _H__COM_DIAG_UNITTEST_S3_OBJECT

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
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/hayloft/Size.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/BucketTest.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Access.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ObjectBaseTest;

TEST_F(ObjectBaseTest, Heap) {
	Session session;
	Bucket * bucket = new Bucket(session, "ObjectBaseTestHeap");
	Object * object = new Object(*bucket, "ObjectBaseTestHeapKey");
	ASSERT_NE(object, (Object*)0);
	EXPECT_TRUE((*object) == true);
	EXPECT_FALSE(object->isBusy());
	EXPECT_FALSE(object->isRetryable());
	EXPECT_EQ(object->getStatus(), ::S3StatusOK);
	EXPECT_NE(object->getKey(), (char *)0);
	delete object;
	delete bucket;
}

TEST_F(ObjectBaseTest, Stack) {
	Session session;
	Bucket bucket(session, "ObjectBaseTestStack");
	Object object(bucket, "ObjectBaseTestStackKey");
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	Session session;
	EXPECT_TRUE(Object(Bucket(session, "ObjectBaseTestTemporary"), "ObjectBaseTestTemporaryKey") == true);
}

typedef Verbose ObjectPutTest;

TEST_F(ObjectPutTest, Sanity) {
	const char BUCKET[] = "ObjectPutTestSanity";
	const char OBJECT[] = __FILE__;
	Session session;
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	Bucket * bucket;
	BucketTest * buckettest;
	bucket = buckettest = new BucketTest(session, BUCKET, context);
	ASSERT_NE(bucket, (Bucket*)0);
	ASSERT_EQ(*bucket, true);
	ASSERT_FALSE(buckettest->isExistent());
	ASSERT_FALSE(buckettest->isInaccessible());
	ASSERT_TRUE(buckettest->isNonexistent());
	delete bucket;
	BucketCreate * bucketcreate;
	bucket = bucketcreate = new BucketCreate(session, BUCKET, context);
	ASSERT_NE(bucket, (Bucket*)0);
	ASSERT_EQ(*bucket, true);
	ASSERT_TRUE(bucketcreate->isCreated());
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	ASSERT_NE(input, (::com::diag::desperado::PathInput*)0);
	Size octets = size(*input);
	ASSERT_TRUE(octets > 0);
	Object * object = new ObjectPut(*bucket, OBJECT, octets, input, properties);
	ASSERT_EQ(*object, true);
	// http://objectputtestsanity.hayloft.diag.com.s3.amazonaws.com/Object.h
	delete object;
	delete bucket;
	BucketDelete * bucketdelete;
	bucket = bucketdelete = new BucketDelete(session, BUCKET);
	ASSERT_NE(bucket, (Bucket*)0);
	ASSERT_EQ(*bucket, true);
	ASSERT_TRUE(bucketdelete->isDeleted());
	delete bucket;
}

}
}
}
}

#endif
