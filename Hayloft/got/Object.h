/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_OBJECT_
#define _H__COM_DIAG_UNITTEST_S3_OBJECT_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/hayloft/ObjectGet.h"
#include "com/diag/grandote/PathOutput.h"
#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/hayloft/ObjectHead.h"
#include "com/diag/hayloft/ObjectCopy.h"
#include "com/diag/hayloft/size.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/BucketHead.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/hayloft/BucketManifest.h"
#include "com/diag/hayloft/Multiplex.h"
#include "com/diag/hayloft/Simplex.h"
#include "com/diag/hayloft/Context.h"
#include "com/diag/hayloft/Access.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/stdlib.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
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

typedef Fixture ObjectTest;

TEST_F(ObjectTest, Heap) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "ObjectTestHeap";
	static const char OBJECT[] = "Object.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	/**/
	BucketHead * buckethead = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete buckethead;
		buckethead = new BucketHead(BUCKET, context);
		ASSERT_NE(buckethead, (BucketHead*)0);
		EXPECT_EQ(*buckethead, true);
		if (!buckethead->isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckethead->isIdle());
	EXPECT_FALSE(buckethead->isBusy());
	EXPECT_FALSE(buckethead->isRetryable());
	EXPECT_FALSE(buckethead->isInaccessible());
	EXPECT_TRUE(buckethead->isNonexistent());
	ASSERT_FALSE(buckethead->isSuccessful());
	delete buckethead;
	/**/
	BucketCreate * bucketcreate = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete bucketcreate;
		bucketcreate = new BucketCreate(BUCKET, context);
		ASSERT_NE(bucketcreate, (Bucket*)0);
		EXPECT_EQ(*bucketcreate, true);
		if (!bucketcreate->isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketcreate->isIdle());
	EXPECT_FALSE(bucketcreate->isBusy());
	EXPECT_FALSE(bucketcreate->isRetryable());
	EXPECT_FALSE(bucketcreate->isInaccessible());
	EXPECT_FALSE(bucketcreate->isNonexistent());
	ASSERT_TRUE(bucketcreate->isSuccessful());
	/**/
	buckethead = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete buckethead;
			buckethead = new BucketHead(BUCKET, context);
			ASSERT_NE(buckethead, (BucketHead*)0);
			EXPECT_EQ(*buckethead, true);
			if (!buckethead->isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (buckethead->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckethead->isIdle());
	EXPECT_FALSE(buckethead->isBusy());
	EXPECT_FALSE(buckethead->isRetryable());
	EXPECT_FALSE(buckethead->isInaccessible());
	EXPECT_FALSE(buckethead->isNonexistent());
	ASSERT_TRUE(buckethead->isSuccessful());
	delete buckethead;
	/**/
	ObjectHead * objecthead = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objecthead;
		objecthead = new ObjectHead(OBJECT, *bucketcreate);
		ASSERT_NE(objecthead, (ObjectHead*)0);
		EXPECT_EQ(*objecthead, true);
		if (!objecthead->isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_TRUE(objecthead->isNonexistent());
	ASSERT_FALSE(objecthead->isSuccessful());
	delete objecthead;
	/**/
	ObjectPut * objectput = 0;
	Size inputsize = 0;
	for (int ii = 0; ii < LIMIT; ++ii) {
		delete objectput;
		::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		ASSERT_TRUE(inputsize > 0);
		objectput = new ObjectPut(OBJECT, *bucketcreate, input /* TAKEN */, inputsize, properties);
		ASSERT_NE(objectput, (ObjectPut*)0);
		EXPECT_EQ(*objectput, true);
		if (!objectput->isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectput->isIdle());
	EXPECT_FALSE(objectput->isBusy());
	EXPECT_FALSE(objectput->isRetryable());
	EXPECT_FALSE(objectput->isInaccessible());
	EXPECT_FALSE(objectput->isNonexistent());
	ASSERT_TRUE(objectput->isSuccessful());
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
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_FALSE(objecthead->isNonexistent());
	ASSERT_TRUE(objecthead->isSuccessful());
	ASSERT_NE(objecthead->authenticated(), (char *)0);
	logger.configuration("URL=\"%s\"\n", objecthead->authenticated());
	delete objecthead;
	/* http://objecttest.hayloft.diag.com.s3.amazonaws.com/SynchronousHeap.txt */
	ObjectGet * objectget = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete objectget;
			::com::diag::grandote::PathOutput * output = new ::com::diag::grandote::PathOutput(OBJECT);
			ASSERT_NE(output, (::com::diag::grandote::PathOutput*)0);
			objectget = new ObjectGet(OBJECT, *bucketcreate, output /* TAKEN */);
			ASSERT_NE(objectget, (ObjectGet*)0);
			EXPECT_EQ(*objectget, true);
			if (!objectget->isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objectget->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectget->isIdle());
	EXPECT_FALSE(objectget->isBusy());
	EXPECT_FALSE(objectget->isRetryable());
	EXPECT_FALSE(objectget->isInaccessible());
	EXPECT_FALSE(objectget->isNonexistent());
	ASSERT_TRUE(objectget->isSuccessful());
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
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_FALSE(objecthead->isNonexistent());
	ASSERT_TRUE(objecthead->isSuccessful());
	delete objecthead;
	/**/
	ObjectDelete * objectdelete = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete objectdelete;
			objectdelete = new ObjectDelete(OBJECT, *bucketcreate);
			ASSERT_NE(objectdelete, (ObjectDelete*)0);
			EXPECT_EQ(*objectdelete, true);
			if (!objectdelete->isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objectdelete->isNonexistent()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectdelete->isIdle());
	EXPECT_FALSE(objectdelete->isBusy());
	EXPECT_FALSE(objectdelete->isRetryable());
	EXPECT_FALSE(objectdelete->isInaccessible());
	EXPECT_FALSE(objectdelete->isNonexistent());
	ASSERT_TRUE(objectdelete->isSuccessful());
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
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objecthead->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead->isIdle());
	EXPECT_FALSE(objecthead->isBusy());
	EXPECT_FALSE(objecthead->isRetryable());
	EXPECT_FALSE(objecthead->isInaccessible());
	EXPECT_TRUE(objecthead->isNonexistent());
	ASSERT_FALSE(objecthead->isSuccessful());
	delete objecthead;
	delete bucketcreate;
	/**/
	BucketDelete * bucketdelete = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete bucketdelete;
			bucketdelete = new BucketDelete(BUCKET);
			ASSERT_NE(bucketdelete, (Bucket*)0);
			EXPECT_EQ(*bucketdelete, true);
			if (!bucketdelete->isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!bucketdelete->isNonexistent()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketdelete->isIdle());
	EXPECT_FALSE(bucketdelete->isBusy());
	EXPECT_FALSE(bucketdelete->isRetryable());
	EXPECT_FALSE(bucketdelete->isInaccessible());
	EXPECT_FALSE(bucketdelete->isNonexistent());
	ASSERT_TRUE(bucketdelete->isSuccessful());
	delete bucketdelete;
	/**/
	buckethead = 0;
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			delete buckethead;
			buckethead = new BucketHead(BUCKET, context);
			ASSERT_NE(buckethead, (BucketHead*)0);
			EXPECT_EQ(*buckethead, true);
			if (!buckethead->isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!buckethead->isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(buckethead->isIdle());
	EXPECT_FALSE(buckethead->isBusy());
	EXPECT_FALSE(buckethead->isRetryable());
	EXPECT_FALSE(buckethead->isInaccessible());
	EXPECT_TRUE(buckethead->isNonexistent());
	ASSERT_FALSE(buckethead->isSuccessful());
	delete buckethead;
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

TEST_F(ObjectTest, Complete) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "ObjectTestComplete";
	static const char OBJECT[] = "Object.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	Multiplex multiplex;
	/**/
	BucketHead buckethead(BUCKET, multiplex, context);
	EXPECT_EQ(buckethead, false);
	EXPECT_TRUE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(buckethead.start());
		EXPECT_EQ(buckethead, false);
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_TRUE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		EXPECT_EQ(multiplex.complete(), 0);
		EXPECT_EQ(buckethead, true);
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
	/**/
	BucketCreate bucketcreate(BUCKET, multiplex, context);
	EXPECT_EQ(bucketcreate, false);
	EXPECT_TRUE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	EXPECT_FALSE(bucketcreate.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(bucketcreate.start());
		EXPECT_EQ(bucketcreate, false);
		EXPECT_FALSE(bucketcreate.isIdle());
		EXPECT_TRUE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		EXPECT_FALSE(bucketcreate.isSuccessful());
		EXPECT_EQ(multiplex.complete(), 0);
		EXPECT_EQ(bucketcreate, true);
		if (!bucketcreate.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_TRUE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, false);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(buckethead, true);
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
	/**/
	ObjectHead objecthead(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objecthead, false);
	EXPECT_TRUE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objecthead.start());
		EXPECT_EQ(objecthead, false);
		EXPECT_FALSE(objecthead.isIdle());
		EXPECT_TRUE(objecthead.isBusy());
		EXPECT_FALSE(objecthead.isRetryable());
		EXPECT_FALSE(objecthead.isInaccessible());
		EXPECT_FALSE(objecthead.isNonexistent());
		EXPECT_FALSE(objecthead.isSuccessful());
		EXPECT_EQ(multiplex.complete(), 0);
		EXPECT_EQ(objecthead, true);
		if (!objecthead.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize, properties);
	EXPECT_EQ(objectput, false);
	EXPECT_TRUE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	EXPECT_FALSE(objectput.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objectput.start());
		EXPECT_EQ(objectput, false);
		EXPECT_FALSE(objectput.isIdle());
		EXPECT_TRUE(objectput.isBusy());
		EXPECT_FALSE(objectput.isRetryable());
		EXPECT_FALSE(objectput.isInaccessible());
		EXPECT_FALSE(objectput.isNonexistent());
		EXPECT_FALSE(objectput.isSuccessful());
		EXPECT_EQ(multiplex.complete(), 0);
		EXPECT_EQ(objectput, true);
		if (!objectput.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		EXPECT_TRUE(inputsize > 0);
		objectput.reset(input, inputsize);
	}
	EXPECT_FALSE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	ASSERT_TRUE(objectput.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	ASSERT_NE(objecthead.authenticated(), (char *)0);
	logger.configuration("URL=\"%s\"\n", objecthead.authenticated());
	/* http://objecttest.hayloft.diag.com.s3.amazonaws.com/AsynchronousStackComplete.txt */
	::com::diag::grandote::PathOutput * output = new ::com::diag::grandote::PathOutput(OBJECT);
	ObjectGet objectget(OBJECT, bucketcreate, multiplex, output);
	EXPECT_EQ(objectget, false);
	EXPECT_TRUE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_FALSE(objectget.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectget.start());
			EXPECT_EQ(objectget, false);
			EXPECT_FALSE(objectget.isIdle());
			EXPECT_TRUE(objectget.isBusy());
			EXPECT_FALSE(objectget.isRetryable());
			EXPECT_FALSE(objectget.isInaccessible());
			EXPECT_FALSE(objectget.isNonexistent());
			EXPECT_FALSE(objectget.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(objectget, true);
			if (!objectget.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			output = new ::com::diag::grandote::PathOutput(OBJECT);
			objectget.reset(output);
		}
		if (objectget.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_TRUE(objectget.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	/**/
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objectdelete, false);
	EXPECT_TRUE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	EXPECT_FALSE(objectdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectdelete.start());
			EXPECT_EQ(objectdelete, false);
			EXPECT_FALSE(objectdelete.isIdle());
			EXPECT_TRUE(objectdelete.isBusy());
			EXPECT_FALSE(objectdelete.isRetryable());
			EXPECT_FALSE(objectdelete.isInaccessible());
			EXPECT_FALSE(objectdelete.isNonexistent());
			EXPECT_FALSE(objectdelete.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(objectdelete, true);
			if (!objectdelete.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objectdelete.isNonexistent()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	ASSERT_TRUE(objectdelete.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	BucketDelete bucketdelete(BUCKET, multiplex);
	EXPECT_EQ(bucketdelete, false);
	EXPECT_TRUE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	EXPECT_FALSE(bucketdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(bucketdelete.start());
			EXPECT_EQ(bucketdelete, false);
			EXPECT_FALSE(bucketdelete.isIdle());
			EXPECT_TRUE(bucketdelete.isBusy());
			EXPECT_FALSE(bucketdelete.isRetryable());
			EXPECT_FALSE(bucketdelete.isInaccessible());
			EXPECT_FALSE(bucketdelete.isNonexistent());
			EXPECT_FALSE(bucketdelete.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(bucketdelete, true);
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
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_TRUE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, false);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			EXPECT_EQ(multiplex.complete(), 0);
			EXPECT_EQ(buckethead, true);
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
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

TEST_F(ObjectTest, Simplex) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "ObjectTestSimplex";
	static const char OBJECT[] = "Object.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	Simplex simplex;
	/**/
	BucketHead buckethead(BUCKET, simplex, context);
	EXPECT_EQ(buckethead, false);
	EXPECT_TRUE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(buckethead.start());
		EXPECT_EQ(buckethead, true);
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
	/**/
	BucketCreate bucketcreate(BUCKET, simplex, context);
	EXPECT_EQ(bucketcreate, false);
	EXPECT_TRUE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	EXPECT_FALSE(bucketcreate.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(bucketcreate.start());
		EXPECT_EQ(bucketcreate, true);
		if (!bucketcreate.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_TRUE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, true);
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
	/**/
	ObjectHead objecthead(OBJECT, bucketcreate, simplex);
	EXPECT_EQ(objecthead, false);
	EXPECT_TRUE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objecthead.start());
		EXPECT_EQ(objecthead, true);
		if (!objecthead.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput(OBJECT, bucketcreate, simplex, input, inputsize, properties);
	EXPECT_EQ(objectput, false);
	EXPECT_TRUE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	EXPECT_FALSE(objectput.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objectput.start());
		EXPECT_EQ(objectput, true);
		if (!objectput.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		EXPECT_TRUE(inputsize > 0);
		objectput.reset(input, inputsize);
	}
	EXPECT_FALSE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	ASSERT_TRUE(objectput.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	ASSERT_NE(objecthead.authenticated(), (char *)0);
	logger.configuration("URL=\"%s\"\n", objecthead.authenticated());
	/* http://objecttest.hayloft.diag.com.s3.amazonaws.com/AsynchronousStackComplete.txt */
	::com::diag::grandote::PathOutput * output = new ::com::diag::grandote::PathOutput(OBJECT);
	ObjectGet objectget(OBJECT, bucketcreate, simplex, output);
	EXPECT_EQ(objectget, false);
	EXPECT_TRUE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_FALSE(objectget.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectget.start());
			EXPECT_EQ(objectget, true);
			if (!objectget.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			output = new ::com::diag::grandote::PathOutput(OBJECT);
			objectget.reset(output);
		}
		if (objectget.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_TRUE(objectget.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	/**/
	ObjectDelete objectdelete(OBJECT, bucketcreate, simplex);
	EXPECT_EQ(objectdelete, false);
	EXPECT_TRUE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	EXPECT_FALSE(objectdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectdelete.start());
			EXPECT_EQ(objectdelete, true);
			if (!objectdelete.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objectdelete.isNonexistent()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	ASSERT_TRUE(objectdelete.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	BucketDelete bucketdelete(BUCKET, simplex);
	EXPECT_EQ(bucketdelete, false);
	EXPECT_TRUE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	EXPECT_FALSE(bucketdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(bucketdelete.start());
			EXPECT_EQ(bucketdelete, true);
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
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_TRUE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, true);
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
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

TEST_F(ObjectTest, Service) {
	static const Milliseconds TIMEOUT = 1000;
	static const int LIMIT = 60;
	static const char BUCKET[] = "ObjectTestService";
	static const char OBJECT[] = "Object.txt";
	AccessPublicRead access;
	Context context;
	context.setAccess(access);
	Properties properties;
	properties.setAccess(access);
	Multiplex multiplex;
	/**/
	BucketHead buckethead(BUCKET, multiplex, context);
	EXPECT_EQ(buckethead, false);
	EXPECT_TRUE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(buckethead.start());
		EXPECT_EQ(buckethead, false);
		EXPECT_FALSE(buckethead.isIdle());
		EXPECT_TRUE(buckethead.isBusy());
		EXPECT_FALSE(buckethead.isRetryable());
		EXPECT_FALSE(buckethead.isInaccessible());
		EXPECT_FALSE(buckethead.isNonexistent());
		EXPECT_FALSE(buckethead.isSuccessful());
		int bits = 0;
		for (int kk = 0; (buckethead != true) && (kk < LIMIT); ++kk) {
			if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
			logger.configuration("TIMEDOUT %d\n", __LINE__);
		}
		EXPECT_EQ(bits, 0);
		EXPECT_EQ(buckethead, true);
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
	/**/
	BucketCreate bucketcreate(BUCKET, multiplex, context);
	EXPECT_EQ(bucketcreate, false);
	EXPECT_TRUE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	EXPECT_FALSE(bucketcreate.isNonexistent());
	EXPECT_FALSE(bucketcreate.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(bucketcreate.start());
		EXPECT_EQ(bucketcreate, false);
		EXPECT_FALSE(bucketcreate.isIdle());
		EXPECT_TRUE(bucketcreate.isBusy());
		EXPECT_FALSE(bucketcreate.isRetryable());
		EXPECT_FALSE(bucketcreate.isInaccessible());
		EXPECT_FALSE(bucketcreate.isNonexistent());
		EXPECT_FALSE(bucketcreate.isSuccessful());
		int bits = 0;
		for (int kk = 0; (bucketcreate != true) && (kk < LIMIT); ++kk) {
			if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
			logger.configuration("TIMEDOUT %d\n", __LINE__);
		}
		EXPECT_EQ(bits, 0);
		EXPECT_EQ(bucketcreate, true);
		if (!bucketcreate.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(bucketcreate.isIdle());
	EXPECT_FALSE(bucketcreate.isBusy());
	EXPECT_FALSE(bucketcreate.isRetryable());
	EXPECT_FALSE(bucketcreate.isInaccessible());
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_TRUE(buckethead.isNonexistent());
	EXPECT_FALSE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, false);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			int bits = 0;
			for (int kk = 0; (buckethead != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(buckethead, true);
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
	/**/
	ObjectHead objecthead(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objecthead, false);
	EXPECT_TRUE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objecthead.start());
		EXPECT_EQ(objecthead, false);
		EXPECT_FALSE(objecthead.isIdle());
		EXPECT_TRUE(objecthead.isBusy());
		EXPECT_FALSE(objecthead.isRetryable());
		EXPECT_FALSE(objecthead.isInaccessible());
		EXPECT_FALSE(objecthead.isNonexistent());
		EXPECT_FALSE(objecthead.isSuccessful());
		int bits = 0;
		for (int kk = 0; (objecthead != true) && (kk < LIMIT); ++kk) {
			if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
			logger.configuration("TIMEDOUT %d\n", __LINE__);
		}
		EXPECT_EQ(bits, 0);
		EXPECT_EQ(objecthead, true);
		if (!objecthead.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize, properties);
	EXPECT_EQ(objectput, false);
	EXPECT_TRUE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	EXPECT_FALSE(objectput.isSuccessful());
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(objectput.start());
		EXPECT_EQ(objectput, false);
		EXPECT_FALSE(objectput.isIdle());
		EXPECT_TRUE(objectput.isBusy());
		EXPECT_FALSE(objectput.isRetryable());
		EXPECT_FALSE(objectput.isInaccessible());
		EXPECT_FALSE(objectput.isNonexistent());
		EXPECT_FALSE(objectput.isSuccessful());
		int bits = 0;
		for (int kk = 0; (objectput != true) && (kk < LIMIT); ++kk) {
			if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
			logger.configuration("TIMEDOUT %d\n", __LINE__);
		}
		EXPECT_EQ(bits, 0);
		EXPECT_EQ(objectput, true);
		if (!objectput.isRetryable()) { break; }
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		EXPECT_TRUE(inputsize > 0);
		objectput.reset(input, inputsize);
	}
	EXPECT_FALSE(objectput.isIdle());
	EXPECT_FALSE(objectput.isBusy());
	EXPECT_FALSE(objectput.isRetryable());
	EXPECT_FALSE(objectput.isInaccessible());
	EXPECT_FALSE(objectput.isNonexistent());
	ASSERT_TRUE(objectput.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	EXPECT_FALSE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			int bits = 0;
			for (int kk = 0; (objecthead != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	ASSERT_NE(objecthead.authenticated(), (char *)0);
	logger.configuration("URL=\"%s\"\n", objecthead.authenticated());
	/* http://objecttest.hayloft.diag.com.s3.amazonaws.com/AsynchronousStackComplete.txt */
	::com::diag::grandote::PathOutput * output = new ::com::diag::grandote::PathOutput(OBJECT);
	ObjectGet objectget(OBJECT, bucketcreate, multiplex, output);
	EXPECT_EQ(objectget, false);
	EXPECT_TRUE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_FALSE(objectget.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectget.start());
			EXPECT_EQ(objectget, false);
			EXPECT_FALSE(objectget.isIdle());
			EXPECT_TRUE(objectget.isBusy());
			EXPECT_FALSE(objectget.isRetryable());
			EXPECT_FALSE(objectget.isInaccessible());
			EXPECT_FALSE(objectget.isNonexistent());
			EXPECT_FALSE(objectget.isSuccessful());
			int bits = 0;
			for (int kk = 0; (objectget != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(objectget, true);
			if (!objectget.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
			output = new ::com::diag::grandote::PathOutput(OBJECT);
			objectget.reset(output);
		}
		if (objectget.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectget.isIdle());
	EXPECT_FALSE(objectget.isBusy());
	EXPECT_FALSE(objectget.isRetryable());
	EXPECT_FALSE(objectget.isInaccessible());
	EXPECT_FALSE(objectget.isNonexistent());
	EXPECT_TRUE(objectget.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			int bits = 0;
			for (int kk = 0; (objecthead != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	ASSERT_TRUE(objecthead.isSuccessful());
	/**/
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	EXPECT_EQ(objectdelete, false);
	EXPECT_TRUE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	EXPECT_FALSE(objectdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objectdelete.start());
			EXPECT_EQ(objectdelete, false);
			EXPECT_FALSE(objectdelete.isIdle());
			EXPECT_TRUE(objectdelete.isBusy());
			EXPECT_FALSE(objectdelete.isRetryable());
			EXPECT_FALSE(objectdelete.isInaccessible());
			EXPECT_FALSE(objectdelete.isNonexistent());
			EXPECT_FALSE(objectdelete.isSuccessful());
			int bits = 0;
			for (int kk = 0; (objectdelete != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(objectdelete, true);
			if (!objectdelete.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objectdelete.isNonexistent()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objectdelete.isIdle());
	EXPECT_FALSE(objectdelete.isBusy());
	EXPECT_FALSE(objectdelete.isRetryable());
	EXPECT_FALSE(objectdelete.isInaccessible());
	EXPECT_FALSE(objectdelete.isNonexistent());
	ASSERT_TRUE(objectdelete.isSuccessful());
	/**/
	EXPECT_EQ(objecthead, true);
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_FALSE(objecthead.isNonexistent());
	EXPECT_TRUE(objecthead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(objecthead.start());
			EXPECT_EQ(objecthead, false);
			EXPECT_FALSE(objecthead.isIdle());
			EXPECT_TRUE(objecthead.isBusy());
			EXPECT_FALSE(objecthead.isRetryable());
			EXPECT_FALSE(objecthead.isInaccessible());
			EXPECT_FALSE(objecthead.isNonexistent());
			EXPECT_FALSE(objecthead.isSuccessful());
			int bits = 0;
			for (int kk = 0; (objecthead != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(objecthead, true);
			if (!objecthead.isRetryable()) { break; }
			logger.configuration("RETRYING %d\n", __LINE__);
			platform.yield(platform.frequency());
		}
		if (!objecthead.isSuccessful()) { break; }
		logger.configuration("WAITING %d\n", __LINE__);
		platform.yield(platform.frequency());
	}
	EXPECT_FALSE(objecthead.isIdle());
	EXPECT_FALSE(objecthead.isBusy());
	EXPECT_FALSE(objecthead.isRetryable());
	EXPECT_FALSE(objecthead.isInaccessible());
	EXPECT_TRUE(objecthead.isNonexistent());
	ASSERT_FALSE(objecthead.isSuccessful());
	/**/
	BucketDelete bucketdelete(BUCKET, multiplex);
	EXPECT_EQ(bucketdelete, false);
	EXPECT_TRUE(bucketdelete.isIdle());
	EXPECT_FALSE(bucketdelete.isBusy());
	EXPECT_FALSE(bucketdelete.isRetryable());
	EXPECT_FALSE(bucketdelete.isInaccessible());
	EXPECT_FALSE(bucketdelete.isNonexistent());
	EXPECT_FALSE(bucketdelete.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(bucketdelete.start());
			EXPECT_EQ(bucketdelete, false);
			EXPECT_FALSE(bucketdelete.isIdle());
			EXPECT_TRUE(bucketdelete.isBusy());
			EXPECT_FALSE(bucketdelete.isRetryable());
			EXPECT_FALSE(bucketdelete.isInaccessible());
			EXPECT_FALSE(bucketdelete.isNonexistent());
			EXPECT_FALSE(bucketdelete.isSuccessful());
			int bits = 0;
			for (int kk = 0; (bucketdelete != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(bucketdelete, true);
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
	/**/
	EXPECT_EQ(buckethead, true);
	EXPECT_FALSE(buckethead.isIdle());
	EXPECT_FALSE(buckethead.isBusy());
	EXPECT_FALSE(buckethead.isRetryable());
	EXPECT_FALSE(buckethead.isInaccessible());
	EXPECT_FALSE(buckethead.isNonexistent());
	EXPECT_TRUE(buckethead.isSuccessful());
	for (int jj = 0; jj < LIMIT; ++jj) {
		for (int ii = 0; ii < LIMIT; ++ii) {
			EXPECT_TRUE(buckethead.start());
			EXPECT_EQ(buckethead, false);
			EXPECT_FALSE(buckethead.isIdle());
			EXPECT_TRUE(buckethead.isBusy());
			EXPECT_FALSE(buckethead.isRetryable());
			EXPECT_FALSE(buckethead.isInaccessible());
			EXPECT_FALSE(buckethead.isNonexistent());
			EXPECT_FALSE(buckethead.isSuccessful());
			int bits = 0;
			for (int kk = 0; (buckethead != true) && (kk < LIMIT); ++kk) {
				if ((bits = multiplex.service(TIMEOUT, LIMIT)) <= 0) { break; }
				logger.configuration("TIMEDOUT %d\n", __LINE__);
			}
			EXPECT_EQ(bits, 0);
			EXPECT_EQ(buckethead, true);
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
	/**/
	Size outputsize = size(OBJECT);
	EXPECT_EQ(inputsize, outputsize);
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT), 0);
}

TEST_F(ObjectTest, Manifest) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "ObjectTestManifest";
	static const char OBJECT1[] = "Object1.txt";
	static const char OBJECT2[] = "Object2.txt";
	BucketCreate bucketcreate(BUCKET);
	for (int ii = 0; bucketcreate.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate.start());
	}
	ASSERT_TRUE(bucketcreate.isSuccessful());
	/**/
	BucketManifest bucketmanifest1(BUCKET);
	for (int ii = 0; (bucketmanifest1.isRetryable() || bucketmanifest1.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest1.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest1.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest1.start());
	}
	ASSERT_TRUE(bucketmanifest1.isSuccessful());
	EXPECT_EQ(bucketmanifest1.getManifest().size(), 0);
	EXPECT_EQ(bucketmanifest1.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_EQ(bucketmanifest1.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
	Size inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput1(OBJECT1, bucketcreate, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		EXPECT_TRUE(inputsize > 0);
		objectput1.reset(input, inputsize);
		EXPECT_TRUE(objectput1.start());
	}
	ASSERT_TRUE(objectput1.isSuccessful());
	/**/
	BucketManifest bucketmanifest2(BUCKET);
	for (int ii = 0; (bucketmanifest2.isRetryable() || bucketmanifest2.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest2.start());
	}
	ASSERT_TRUE(bucketmanifest2.isSuccessful());
	EXPECT_EQ(bucketmanifest2.getManifest().size(), 1);
	EXPECT_NE(bucketmanifest2.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_EQ(bucketmanifest2.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	input = new ::com::diag::grandote::PathInput("unittest.txt");
	ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
	inputsize = size(*input);
	EXPECT_TRUE(inputsize > 0);
	ObjectPut objectput2(OBJECT2, bucketcreate, input, inputsize);
	for (int ii = 0; objectput2.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		ASSERT_NE(input, (::com::diag::grandote::PathInput*)0);
		inputsize = size(*input);
		EXPECT_TRUE(inputsize > 0);
		objectput2.reset(input, inputsize);
		EXPECT_TRUE(objectput2.start());
	}
	ASSERT_TRUE(objectput2.isSuccessful());
	/**/
	BucketManifest bucketmanifest3(BUCKET);
	for (int ii = 0; (bucketmanifest3.isRetryable() || bucketmanifest3.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest3.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest3.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest3.start());
	}
	ASSERT_TRUE(bucketmanifest3.isSuccessful());
	EXPECT_EQ(bucketmanifest3.getManifest().size(), 2);
	EXPECT_NE(bucketmanifest3.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_NE(bucketmanifest3.find(OBJECT2), (BucketManifest::Entry *)0);
	show(bucketmanifest3);
	EXPECT_TRUE(bucketmanifest3.reset());
	EXPECT_EQ(bucketmanifest3.getManifest().size(), 0);
	EXPECT_EQ(bucketmanifest3.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_EQ(bucketmanifest3.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	Selection selection4(0, 0, 0, 1);
	BucketManifest bucketmanifest4(BUCKET, selection4);
	for (int ii = 0; (bucketmanifest4.isRetryable() || bucketmanifest4.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest4.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest4.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest4.start());
	}
	ASSERT_TRUE(bucketmanifest4.isSuccessful());
	EXPECT_EQ(bucketmanifest4.getManifest().size(), 1);
	EXPECT_NE(bucketmanifest4.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_EQ(bucketmanifest4.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	Selection selection5("Object2");
	BucketManifest bucketmanifest5(BUCKET, selection5);
	for (int ii = 0; (bucketmanifest5.isRetryable() || bucketmanifest5.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest5.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest5.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest5.start());
	}
	ASSERT_TRUE(bucketmanifest5.isSuccessful());
	EXPECT_EQ(bucketmanifest5.getManifest().size(), 1);
	EXPECT_EQ(bucketmanifest5.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_NE(bucketmanifest5.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	Selection selection6("Object");
	BucketManifest bucketmanifest6(BUCKET, selection6);
	for (int ii = 0; (bucketmanifest6.isRetryable() || bucketmanifest6.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest6.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest6.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest6.start());
	}
	ASSERT_TRUE(bucketmanifest6.isSuccessful());
	EXPECT_EQ(bucketmanifest6.getManifest().size(), 2);
	EXPECT_NE(bucketmanifest6.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_NE(bucketmanifest6.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	Selection selection7(0, "Object2");
	BucketManifest bucketmanifest7(BUCKET, selection7);
	for (int ii = 0; (bucketmanifest7.isRetryable() || bucketmanifest7.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest7.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest7.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest7.start());
	}
	ASSERT_TRUE(bucketmanifest7.isSuccessful());
	EXPECT_EQ(bucketmanifest7.getManifest().size(), 1);
	EXPECT_EQ(bucketmanifest7.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_NE(bucketmanifest7.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	Selection selection8("Ob", 0, "ct");
	BucketManifest bucketmanifest8(BUCKET, selection8);
	for (int ii = 0; (bucketmanifest8.isRetryable() || bucketmanifest8.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest8.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest8.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest8.start());
	}
	ASSERT_TRUE(bucketmanifest8.isSuccessful());
	EXPECT_EQ(bucketmanifest8.getManifest().size(), 0);
	EXPECT_EQ(bucketmanifest8.getCommon().size(), 1);
	EXPECT_EQ(bucketmanifest8.getCommon().front(), "Object");
	/**/
	ObjectDelete objectdelete1(OBJECT1, bucketcreate);
	for (int ii = 0; (objectdelete1.isRetryable() || objectdelete1.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectdelete1.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objectdelete1.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectdelete1.start());
	}
	ASSERT_TRUE(objectdelete1.isSuccessful());
	/**/
	BucketManifest bucketmanifest9(BUCKET);
	for (int ii = 0; (bucketmanifest9.isRetryable() || bucketmanifest9.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest9.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest9.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest9.start());
	}
	ASSERT_TRUE(bucketmanifest9.isSuccessful());
	EXPECT_EQ(bucketmanifest9.getManifest().size(), 1);
	EXPECT_EQ(bucketmanifest9.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_NE(bucketmanifest9.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
	ObjectDelete objectdelete2(OBJECT2, bucketcreate);
	for (int ii = 0; (objectdelete2.isRetryable() || objectdelete2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectdelete2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objectdelete2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectdelete2.start());
	}
	ASSERT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketManifest bucketmanifest10(BUCKET);
	for (int ii = 0; (bucketmanifest10.isRetryable() || bucketmanifest10.isNonexistent()) && (ii < LIMIT); ++ii) {
		if (bucketmanifest10.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (bucketmanifest10.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketmanifest10.start());
	}
	ASSERT_TRUE(bucketmanifest10.isSuccessful());
	EXPECT_EQ(bucketmanifest10.getManifest().size(), 0);
	EXPECT_EQ(bucketmanifest10.find(OBJECT1), (BucketManifest::Entry *)0);
	EXPECT_EQ(bucketmanifest10.find(OBJECT2), (BucketManifest::Entry *)0);
	/**/
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
	ASSERT_TRUE(bucketdelete.isSuccessful());
}

TEST_F(ObjectTest, Copy) {
	// I'm also experimenting to see what meta-data is returned by which
	// OBJECT ACTION. It looks like a full set of Response Properties are
	// only returned by OBJECT HEAD and OBJECT GET.
	static const char BUCKET1[] = "ObjectTestCopy1";
	static const char BUCKET2[] = "ObjectTestCopy2";
	static const char OBJECT1[] = "Object1.txt";
	static const char OBJECT2[] = "Object2.txt";
	static const int LIMIT = 10;
	BucketCreate bucketcreate1(BUCKET1);
	for (int ii = 0; bucketcreate1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate1.start());
	}
	ASSERT_TRUE(bucketcreate1.isSuccessful());
	/**/
	BucketCreate bucketcreate2(BUCKET2);
	for (int ii = 0; bucketcreate2.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(bucketcreate2.start());
	}
	ASSERT_TRUE(bucketcreate2.isSuccessful());
	show(bucketcreate1);
	/**/
	Properties properties;
	properties.insert("KeywordA", "ValueA").insert("KeywordB", "ValueB");
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, bucketcreate1, input, inputsize, properties);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
		EXPECT_TRUE(objectput1.start());
	}
	ASSERT_TRUE(objectput1.isSuccessful());
	show(objectput1);
	/**/
	ObjectHead objecthead1(OBJECT1, bucketcreate1);
	for (int ii = 0; (objecthead1.isRetryable() || objecthead1.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objecthead1.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objecthead1.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objecthead1.start());
	}
	ASSERT_TRUE(objecthead1.isSuccessful());
	show(objecthead1);
	// Note that S3 converts the keyword strings to lower case. That's because
	// they actually become part of HTTP message header field names whose
	// prefixes libs3 strips off.
	ASSERT_NE(objecthead1.find("keyworda"), (char *)0);
	EXPECT_EQ(std::strcmp(objecthead1.find("keyworda"), "ValueA"), 0);
	ASSERT_NE(objecthead1.find("keywordb"), (char *)0);
	EXPECT_EQ(std::strcmp(objecthead1.find("keywordb"), "ValueB"), 0);
	/**/
	// I could have just used bucketcreate1 and bucketcreate2 but I wanted to
	// try just using the base class since I intended it to be used this way.
	Bucket bucket1(BUCKET1);
	Bucket bucket2(BUCKET2);
	ObjectCopy objectcopy(OBJECT1, bucket1, OBJECT2, bucket2);
	for (int ii = 0; objectcopy.isRetryable() && (ii < LIMIT); ++ii) {
		logger.configuration("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectcopy.start());
	}
	ASSERT_TRUE(objectcopy.isSuccessful());
	// Turning debug logging on suggests that S3 doesn't respond with a
	// response properties for an OBJECT COPY, so most of these fields don't
	// get filled in. libs3.h suggests that its S3_object_copy() functions
	// provides eTag and modificationTime for the new OBJECT. But unless there's
	// a bug in my code (entirely possible) that doesn't seem to happen either.
	// If you want them, you're going to have to do an OBJECT HEAD.
	/**/
	ObjectHead objecthead2(OBJECT2, bucket2);
	for (int ii = 0; (objecthead2.isRetryable() || objecthead2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objecthead2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objecthead2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objecthead2.start());
	}
	ASSERT_TRUE(objecthead2.isSuccessful());
	show(objecthead2);
	// Note that S3 does not copy the metadata from the source object to the
	// sink object.
	EXPECT_EQ(objecthead2.find("keyworda"), (char *)0);
	EXPECT_EQ(objecthead2.find("keywordb"), (char *)0);
	/**/
	::com::diag::grandote::PathOutput * output2 = new ::com::diag::grandote::PathOutput(OBJECT2);
	ObjectGet objectget2(OBJECT2, bucket2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectget2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::grandote::PathOutput(OBJECT2);
			objectget2.reset(output2);
		} else if (objectget2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	ASSERT_TRUE(objectget2.isSuccessful());
	EXPECT_EQ(objectget2.find("keyworda"), (char *)0);
	EXPECT_EQ(objectget2.find("keywordb"), (char *)0);
	show(objectget2);
	/**/
	ObjectDelete objectdelete1(OBJECT1, bucket1);
	for (int ii = 0; (objectdelete1.isRetryable() || objectdelete1.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectdelete1.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objectdelete1.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectdelete1.start());
	}
	ASSERT_TRUE(objectdelete1.isSuccessful());
	/**/
	ObjectDelete objectdelete2(OBJECT2, bucket2);
	for (int ii = 0; (objectdelete2.isRetryable() || objectdelete2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectdelete2.isRetryable()) {
			logger.configuration("RETRYING %d\n", __LINE__);
		} else if (objectdelete2.isNonexistent()) {
			logger.configuration("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectdelete2.start());
	}
	ASSERT_TRUE(objectdelete2.isSuccessful());
	/**/
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
	ASSERT_TRUE(bucketdelete1.isSuccessful());
	/**/
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
	ASSERT_TRUE(bucketdelete2.isSuccessful());
	/**/
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT2;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2), 0);
}

}
}
}
}

#endif
