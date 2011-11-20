/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_OBJECT_
#define _H__COM_DIAG_UNITTEST_S3_OBJECT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/ObjectHead.h"
#include "com/diag/hayloft/Size.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/BucketTest.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Access.h"
#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ObjectBaseTest;

TEST_F(ObjectBaseTest, Heap) {
	Bucket * bucket = new Bucket("ObjectBaseTestHeap");
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
	Bucket bucket("ObjectBaseTestStack");
	Object object(bucket, "ObjectBaseTestStackKey");
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	EXPECT_TRUE(Object(Bucket("ObjectBaseTestTemporary"), "ObjectBaseTestTemporaryKey") == true);
}

typedef Fixture ObjectTest;

TEST_F(ObjectTest, Sanity) {
	const char BUCKET[] = "ObjectTest";
	const char OBJECT[] = "Sanity.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	/**/
	BucketTest * buckettest = new BucketTest(BUCKET, context);
	ASSERT_NE(buckettest, (BucketTest*)0);
	EXPECT_EQ(*buckettest, true);
	EXPECT_FALSE(buckettest->isExistent());
	EXPECT_FALSE(buckettest->isInaccessible());
	EXPECT_TRUE(buckettest->isNonexistent());
	delete buckettest;
	/**/
	BucketCreate * bucketcreate = new BucketCreate(BUCKET, context);
	ASSERT_NE(bucketcreate, (Bucket*)0);
	EXPECT_EQ(*bucketcreate, true);
	EXPECT_TRUE(bucketcreate->isCreated());
	/**/
	ObjectHead * objecthead = new ObjectHead(*bucketcreate, OBJECT);
	ASSERT_NE(objecthead, (ObjectHead*)0);
	EXPECT_EQ(*objecthead, true);
	EXPECT_FALSE(objecthead->isExistent());
	EXPECT_TRUE(objecthead->isNonexistent());
	delete objecthead;
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	ASSERT_NE(input, (::com::diag::desperado::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut * objectput = new ObjectPut(*bucketcreate, OBJECT, input, inputsize, properties);
	EXPECT_NE(objectput, (ObjectPut*)0);
	EXPECT_EQ(*objectput, true);
	EXPECT_TRUE(objectput->isPut());
	delete objectput;
	/**/
	objecthead = new ObjectHead(*bucketcreate, OBJECT);
	ASSERT_NE(objecthead, (ObjectHead*)0);
	EXPECT_EQ(*objecthead, true);
	EXPECT_TRUE(objecthead->isExistent());
	EXPECT_FALSE(objecthead->isNonexistent());
	delete objecthead;
	/**/
	// http://objectputtestsanity.hayloft.diag.com.s3.amazonaws.com/Sanity.txt
	/**/
	::com::diag::desperado::PathOutput * output = new ::com::diag::desperado::PathOutput(OBJECT);
	ObjectGet * objectget = new ObjectGet(*bucketcreate, OBJECT, output);
	ASSERT_NE(objectget, (ObjectGet*)0);
	EXPECT_EQ(*objectget, true);
	EXPECT_TRUE(objectget->isGotten());
	delete objectget;
	/**/
	ObjectDelete * objectdelete = new ObjectDelete(*bucketcreate, OBJECT);
	ASSERT_NE(objectdelete, (ObjectDelete*)0);
	EXPECT_EQ(*objectdelete, true);
	EXPECT_TRUE(objectdelete->isDeleted());
	delete objectdelete;
	/**/
	objecthead = new ObjectHead(*bucketcreate, OBJECT);
	ASSERT_NE(objecthead, (ObjectHead*)0);
	EXPECT_EQ(*objecthead, true);
	EXPECT_FALSE(objecthead->isExistent());
	EXPECT_TRUE(objecthead->isNonexistent());
	delete objecthead;
	/**/
	BucketDelete * bucketdelete = new BucketDelete(BUCKET);
	ASSERT_NE(bucketdelete, (Bucket*)0);
	EXPECT_EQ(*bucketdelete, true);
	EXPECT_TRUE(bucketdelete->isDeleted());
	delete bucketdelete;
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += OBJECT;
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

}
}
}
}

#endif
