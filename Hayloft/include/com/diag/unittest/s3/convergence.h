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
#include "com/diag/hayloft/s3/ObjectCopy.h"
#include "com/diag/hayloft/size.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/BucketHead.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Access.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/convergence.h"
#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ConvergenceTest;

TEST_F(ConvergenceTest, Synchronous) {
	const char BUCKET1[] = "ConvergenceTestSynchronous1";
	const char BUCKET2[] = "ConvergenceTestSynchronous2";
	const char OBJECT1[] = "Object1.txt";
	const char OBJECT2[] = "Object2.txt";
	Logger::Level LEVEL = Logger::PRINT;
	/**/
	BucketHead buckethead1(BUCKET1);
	ASSERT_TRUE(complete_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1);
	ASSERT_TRUE(complete_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1);
	ASSERT_TRUE(complete_until_success(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2);
	ASSERT_TRUE(complete_until_success(bucketcreate2, LEVEL));
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, bucketcreate1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < 10); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
		objectput1.start();
	}
	ASSERT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, bucketcreate1);
	ASSERT_TRUE(complete_until_success(objecthead1,LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, bucketcreate1, OBJECT2, bucketcreate2);
	ASSERT_TRUE(complete_until_success(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2, bucketcreate2);
	ASSERT_TRUE(complete_until_success(objecthead2, LEVEL));
	/**/
	::com::diag::desperado::PathOutput * output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
	ObjectGet objectget2(OBJECT2, bucketcreate2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < 10); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
			objectget2.reset(output2);
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		objectget2.start();
	}
	ASSERT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, bucketcreate1);
	ASSERT_TRUE(complete_until_success(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2, bucketcreate2);
	ASSERT_TRUE(complete_until_success(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1);
	ASSERT_TRUE(complete_until_success(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2);
	ASSERT_TRUE(complete_until_success(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += OBJECT2;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2), 0);
}

TEST_F(ConvergenceTest, Complete) {
	const char BUCKET1[] = "ConvergenceTestComplete1";
	const char BUCKET2[] = "ConvergenceTestComplete2";
	const char OBJECT1[] = "Object1.txt";
	const char OBJECT2[] = "Object2.txt";
	Logger::Level LEVEL = Logger::PRINT;
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	ASSERT_TRUE(complete_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	ASSERT_TRUE(complete_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	ASSERT_TRUE(complete_until_success(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	ASSERT_TRUE(complete_until_success(bucketcreate2, LEVEL));
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, bucketcreate1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < 10); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
		objectput1.start();
	}
	ASSERT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, bucketcreate1, multiplex);
	ASSERT_TRUE(complete_until_success(objecthead1, LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, bucketcreate1, OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(complete_until_success(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(complete_until_success(objecthead2, LEVEL));
	/**/
	::com::diag::desperado::PathOutput * output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
	ObjectGet objectget2(OBJECT2, bucketcreate2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < 10); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
			objectget2.reset(output2);
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		objectget2.start();
	}
	ASSERT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, bucketcreate1, multiplex);
	ASSERT_TRUE(complete_until_success(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(complete_until_success(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	ASSERT_TRUE(complete_until_success(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	ASSERT_TRUE(complete_until_success(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += OBJECT2;
	ASSERT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2), 0);
}

TEST_F(ConvergenceTest, Service) {
	const char BUCKET1[] = "ConvergenceTestService1";
	const char BUCKET2[] = "ConvergenceTestService2";
	const char OBJECT1[] = "Object1.txt";
	const char OBJECT2[] = "Object2.txt";
	Logger::Level LEVEL = Logger::PRINT;
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	ASSERT_TRUE(service_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	ASSERT_TRUE(service_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	ASSERT_TRUE(service_until_success(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	ASSERT_TRUE(service_until_success(bucketcreate2, LEVEL));
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, bucketcreate1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < 10); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
		objectput1.start();
	}
	ASSERT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, bucketcreate1, multiplex);
	ASSERT_TRUE(service_until_success(objecthead1, LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, bucketcreate1, OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(service_until_success(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(service_until_success(objecthead2, LEVEL));
	/**/
	::com::diag::desperado::PathOutput * output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
	ObjectGet objectget2(OBJECT2, bucketcreate2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < 10); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::desperado::PathOutput(OBJECT2);
			objectget2.reset(output2);
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		objectget2.start();
	}
	ASSERT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, bucketcreate1, multiplex);
	ASSERT_TRUE(service_until_success(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2, bucketcreate2, multiplex);
	ASSERT_TRUE(service_until_success(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	ASSERT_TRUE(service_until_success(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	ASSERT_TRUE(service_until_success(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += __FILE__;
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
