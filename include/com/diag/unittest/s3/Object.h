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
	Object * object = new Object("ObjectBaseTestHeapKey", *bucket);
	ASSERT_NE(object, (Object*)0);
	EXPECT_TRUE((*object) == true);
	EXPECT_FALSE(object->isIdle());
	EXPECT_FALSE(object->isBusy());
	EXPECT_FALSE(object->isRetryable());
	EXPECT_EQ(object->getStatus(), ::S3StatusOK);
	EXPECT_NE(object->getKey(), (char *)0);
	delete object;
	delete bucket;
}

TEST_F(ObjectBaseTest, Stack) {
	Bucket bucket("ObjectBaseTestStack");
	Object object("ObjectBaseTestStackKey", bucket);
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isIdle());
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	EXPECT_TRUE(Object("ObjectBaseTestTemporaryKey", Bucket("ObjectBaseTestTemporary")) == true);
}

typedef Fixture ObjectTest;

TEST_F(ObjectTest, SynchronousHeap) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "ObjectTest";
	static const char OBJECT[] = "SynchronousHeap.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	/**/
	BucketTest * buckettest = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete buckettest;
		buckettest = new BucketTest(BUCKET, context);
		ASSERT_NE(buckettest, (BucketTest*)0);
		EXPECT_EQ(*buckettest, true);
		if (!buckettest->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckettest->isIdle());
	EXPECT_FALSE(buckettest->isBusy());
	EXPECT_FALSE(buckettest->isRetryable());
	EXPECT_FALSE(buckettest->isInaccessible());
	EXPECT_TRUE(buckettest->isNonexistent());
	ASSERT_FALSE(buckettest->isExistent());
	delete buckettest;
	/**/
	BucketCreate * bucketcreate = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete bucketcreate;
		bucketcreate = new BucketCreate(BUCKET, context);
		ASSERT_NE(bucketcreate, (Bucket*)0);
		EXPECT_EQ(*bucketcreate, true);
		if (!bucketcreate->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketcreate->isIdle());
	EXPECT_FALSE(bucketcreate->isBusy());
	EXPECT_FALSE(bucketcreate->isRetryable());
	EXPECT_FALSE(bucketcreate->isInaccessible());
	EXPECT_FALSE(bucketcreate->isNonexistent());
	ASSERT_TRUE(bucketcreate->isCreated());
	/**/
	buckettest = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete buckettest;
			buckettest = new BucketTest(BUCKET, context);
			ASSERT_NE(buckettest, (BucketTest*)0);
			EXPECT_EQ(*buckettest, true);
			if (!buckettest->isRetryable()) { break; }
			platform.yield(platform.frequency());
		}
		if (buckettest->isExistent()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckettest->isIdle());
	EXPECT_FALSE(buckettest->isBusy());
	EXPECT_FALSE(buckettest->isRetryable());
	EXPECT_FALSE(buckettest->isInaccessible());
	EXPECT_FALSE(buckettest->isNonexistent());
	ASSERT_TRUE(buckettest->isExistent());
	delete buckettest;
	/**/
	ObjectHead * objecthead = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objecthead;
		objecthead = new ObjectHead(OBJECT, *bucketcreate);
		ASSERT_NE(objecthead, (ObjectHead*)0);
		EXPECT_EQ(*objecthead, true);
		if (!objecthead->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_TRUE(objecthead->isNonexistent());
	ASSERT_FALSE(objecthead->isExistent());
	delete objecthead;
	/**/
	ObjectPut * objectput = 0;
	Size inputsize = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objectput;
		::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
		ASSERT_NE(input, (::com::diag::desperado::PathInput*)0);
		inputsize = size(*input);
		ASSERT_TRUE(inputsize > 0);
		objectput = new ObjectPut(OBJECT, *bucketcreate, input /* TAKEN */, inputsize, properties);
		ASSERT_NE(objectput, (ObjectPut*)0);
		EXPECT_EQ(*objectput, true);
		if (!objectput->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectput->isIdle());
	EXPECT_FALSE(objectput->isBusy());
	EXPECT_FALSE(objectput->isRetryable());
	EXPECT_FALSE(objectput->isInaccessible());
	EXPECT_FALSE(objectput->isNonexistent());
	ASSERT_TRUE(objectput->isPut());
	delete objectput;
	/**/
	objecthead = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete objecthead;
			objecthead = new ObjectHead(OBJECT, *bucketcreate);
			ASSERT_NE(objecthead, (ObjectHead*)0);
			EXPECT_EQ(*objecthead, true);
			if (!objecthead->isRetryable()) { break; }
			platform.yield(platform.frequency());
		}
		if (objecthead->isExistent()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_FALSE(objecthead->isNonexistent());
	ASSERT_TRUE(objecthead->isExistent());
	delete objecthead;
	/* http://objectputtestsanity.hayloft.diag.com.s3.amazonaws.com/Synchronous.txt exists. */
	/**/
	ObjectGet * objectget = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objectget;
		::com::diag::desperado::PathOutput * output = new ::com::diag::desperado::PathOutput(OBJECT);
		ASSERT_NE(output, (::com::diag::desperado::PathOutput*)0);
		objectget = new ObjectGet(OBJECT, *bucketcreate, output /* TAKEN */);
		ASSERT_NE(objectget, (ObjectGet*)0);
		EXPECT_EQ(*objectget, true);
		if (!objectget->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectget->isIdle());
	EXPECT_FALSE(objectget->isBusy());
	EXPECT_FALSE(objectget->isRetryable());
	EXPECT_FALSE(objectget->isInaccessible());
	EXPECT_FALSE(objectget->isNonexistent());
	ASSERT_TRUE(objectget->isGotten());
	delete objectget;
	/**/
	objecthead = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete objecthead;
			objecthead = new ObjectHead(OBJECT, *bucketcreate);
			ASSERT_NE(objecthead, (ObjectHead*)0);
			EXPECT_EQ(*objecthead, true);
			if (!objecthead->isRetryable()) { break; }
			platform.yield(platform.frequency());
		}
		if (objecthead->isExistent()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_FALSE(objecthead->isNonexistent());
	ASSERT_TRUE(objecthead->isExistent());
	delete objecthead;
	/**/
	ObjectDelete * objectdelete = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objectdelete;
		objectdelete = new ObjectDelete(OBJECT, *bucketcreate);
		ASSERT_NE(objectdelete, (ObjectDelete*)0);
		EXPECT_EQ(*objectdelete, true);
		if (!objectdelete->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectdelete->isIdle());
	EXPECT_FALSE(objectdelete->isBusy());
	EXPECT_FALSE(objectdelete->isRetryable());
	EXPECT_FALSE(objectdelete->isInaccessible());
	EXPECT_FALSE(objectdelete->isNonexistent());
	ASSERT_TRUE(objectdelete->isDeleted());
	delete objectdelete;
	/**/
	objecthead = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete objecthead;
			objecthead = new ObjectHead(OBJECT, *bucketcreate);
			ASSERT_NE(objecthead, (ObjectHead*)0);
			EXPECT_EQ(*objecthead, true);
			if (!objecthead->isRetryable()) { break; }
			platform.yield(platform.frequency());
		}
		if (objecthead->isNonexistent()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	ASSERT_TRUE(objecthead->isNonexistent());
	delete objecthead;
	/**/
	BucketDelete * bucketdelete = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete bucketdelete;
		bucketdelete = new BucketDelete(BUCKET);
		ASSERT_NE(bucketdelete, (Bucket*)0);
		EXPECT_EQ(*bucketdelete, true);
		if (!bucketdelete->isRetryable()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketdelete->isIdle());
	EXPECT_FALSE(bucketdelete->isBusy());
	EXPECT_FALSE(bucketdelete->isRetryable());
	EXPECT_FALSE(bucketdelete->isInaccessible());
	EXPECT_FALSE(bucketdelete->isNonexistent());
	ASSERT_TRUE(bucketdelete->isDeleted());
	delete bucketdelete;
	/**/
	buckettest = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete buckettest;
			buckettest = new BucketTest(BUCKET, context);
			ASSERT_NE(buckettest, (BucketTest*)0);
			EXPECT_EQ(*buckettest, true);
			if (!buckettest->isRetryable()) { break; }
			platform.yield(platform.frequency());
		}
		if (buckettest->isNonexistent()) { break; }
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckettest->isIdle());
	EXPECT_FALSE(buckettest->isBusy());
	EXPECT_FALSE(buckettest->isRetryable());
	EXPECT_FALSE(buckettest->isInaccessible());
	EXPECT_TRUE(buckettest->isNonexistent());
	ASSERT_FALSE(buckettest->isExistent());
	delete buckettest;
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

TEST_F(ObjectTest, AsynchronousStackComplete) {
	static const char BUCKET[] = "ObjectTest";
	static const char OBJECT[] = "AsynchronousStackComplete.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	Multiplex multiplex;
	/**/
	BucketTest buckettest(BUCKET, multiplex, context);
	EXPECT_EQ(buckettest, false);
	EXPECT_TRUE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	EXPECT_FALSE(buckettest.isExistent());
	buckettest.start();
	EXPECT_EQ(buckettest, false);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_TRUE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	EXPECT_FALSE(buckettest.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(buckettest, true);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_TRUE(buckettest.isNonexistent());
	ASSERT_FALSE(buckettest.isExistent());
	/**/
	BucketCreate bucketcreate(BUCKET, multiplex, context);
	EXPECT_EQ(bucketcreate, false);
	EXPECT_TRUE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	EXPECT_FALSE(bucketcreate.isCreated());
	bucketcreate.start();
	EXPECT_EQ(bucketcreate, false);
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_TRUE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	EXPECT_FALSE(bucketcreate.isCreated());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(bucketcreate, true);
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	ASSERT_TRUE(bucketcreate.isCreated());
	/**/
	EXPECT_EQ(buckettest, true);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_TRUE(buckettest.isNonexistent());
	EXPECT_FALSE(buckettest.isExistent());
	buckettest.start();
	EXPECT_EQ(buckettest, false);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_TRUE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	EXPECT_FALSE(buckettest.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(buckettest, true);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	ASSERT_TRUE(buckettest.isExistent());
	/**/
	ObjectHead objecthead(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objecthead, false);
	EXPECT_TRUE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	objecthead.start();
	EXPECT_EQ(objecthead, false);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_TRUE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isExistent());
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	ASSERT_NE(input, (::com::diag::desperado::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize, properties);
	EXPECT_EQ(objectput, false);
	EXPECT_TRUE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	EXPECT_FALSE(objectput.isPut());
	objectput.start();
	EXPECT_EQ(objectput, false);
	EXPECT_FALSE(objectput.isIdle());
	EXPECT_TRUE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	EXPECT_FALSE(objectput.isPut());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objectput, true);
	EXPECT_FALSE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	ASSERT_TRUE(objectput.isPut());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	objecthead.start();
	EXPECT_EQ(objecthead, false);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_TRUE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isExistent());
	/* http://objectputtestsanity.hayloft.diag.com.s3.amazonaws.com/Asynchronous.txt exists. */
	::com::diag::desperado::PathOutput * output = new ::com::diag::desperado::PathOutput(OBJECT);
	ObjectGet objectget(OBJECT, bucketcreate, multiplex, output);
	EXPECT_EQ(objectget, false);
	EXPECT_TRUE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_FALSE(objectget.isGotten());
	objectget.start();
	EXPECT_EQ(objectget, false);
	EXPECT_FALSE(objectget.isIdle());
	EXPECT_TRUE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_FALSE(objectget.isGotten());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objectget, true);
	EXPECT_FALSE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_TRUE(objectget.isGotten());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isExistent());
	objecthead.start();
	EXPECT_EQ(objecthead, false);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_TRUE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isExistent());
	/**/
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objectdelete, false);
	EXPECT_TRUE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	EXPECT_FALSE(objectdelete.isDeleted());
	objectdelete.start();
	EXPECT_EQ(objectdelete, false);
	EXPECT_FALSE(objectdelete.isIdle());
	EXPECT_TRUE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	EXPECT_FALSE(objectdelete.isDeleted());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objectdelete, true);
	EXPECT_FALSE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	ASSERT_TRUE(objectdelete.isDeleted());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isExistent());
	objecthead.start();
	EXPECT_EQ(objecthead, false);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_TRUE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isExistent());
	/**/
	BucketDelete bucketdelete(BUCKET, multiplex);
	EXPECT_EQ(bucketdelete, false);
	EXPECT_TRUE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	EXPECT_FALSE(bucketdelete.isDeleted());
	bucketdelete.start();
	EXPECT_EQ(bucketdelete, false);
	EXPECT_FALSE(bucketdelete.isIdle());
	EXPECT_TRUE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	EXPECT_FALSE(bucketdelete.isDeleted());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(bucketdelete, true);
	EXPECT_FALSE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	ASSERT_TRUE(bucketdelete.isDeleted());
	/**/
	EXPECT_EQ(buckettest, true);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	EXPECT_TRUE(buckettest.isExistent());
	buckettest.start();
	EXPECT_EQ(buckettest, false);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_TRUE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_FALSE(buckettest.isNonexistent());
	EXPECT_FALSE(buckettest.isExistent());
	EXPECT_TRUE(multiplex.complete());
	EXPECT_EQ(buckettest, true);
	EXPECT_FALSE(buckettest.isIdle());
	EXPECT_FALSE(buckettest.isBusy());
	EXPECT_FALSE(buckettest.isRetryable());
	EXPECT_FALSE(buckettest.isInaccessible());
	EXPECT_TRUE(buckettest.isNonexistent());
	ASSERT_FALSE(buckettest.isExistent());
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

}
}
}
}

#endif
