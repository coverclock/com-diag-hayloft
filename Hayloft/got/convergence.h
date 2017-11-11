/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_CONVERGENCE_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_CONVERGENCE_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/hayloft/ObjectGet.h"
#include "com/diag/grandote/PathOutput.h"
#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/hayloft/ObjectHead.h"
#include "com/diag/hayloft/ObjectCopy.h"
#include "com/diag/grandote/size.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/BucketHead.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/hayloft/BucketManifest.h"
#include "com/diag/hayloft/Context.h"
#include "com/diag/hayloft/Access.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/convergence.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef ::com::diag::hayloft::Object HayloftObject; // Resolve Object ambiguity.

typedef Fixture ConvergenceTest;

TEST_F(ConvergenceTest, Synchronous) {
	Bucket BUCKET1("ConvergenceTestSynchronous1");
	Bucket BUCKET2("ConvergenceTestSynchronous2");
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	/**/
	BucketHead buckethead1(BUCKET1);
	EXPECT_TRUE(complete_until_nonexistent(buckethead1));
	/**/
	BucketHead buckethead2(BUCKET1);
	EXPECT_TRUE(complete_until_nonexistent(buckethead2));
	/**/
	BucketCreate bucketcreate1(BUCKET1);
	EXPECT_TRUE(complete_until_successful(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2);
	EXPECT_TRUE(complete_until_successful(bucketcreate2));
	/**/
	PathInput * input = new PathInput("dat/unittest.txt");
    ASSERT_FALSE(input == (PathInput *)0);
	Size inputsize = size(*input);
    ASSERT_FALSE(inputsize < 0);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("dat/unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1);
	EXPECT_TRUE(complete_until_successful(objecthead1));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2);
	EXPECT_TRUE(complete_until_successful(objectcopy));
	/**/
	ObjectHead objecthead2(OBJECT2);
	EXPECT_TRUE(complete_until_successful(objecthead2));
	/**/
	PathOutput * output2 = new PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectget2.isRetryable()) {
			logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
			output2 = new PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			logger.debug("WAITING %s@%d\n", __FILE__, __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1);
	EXPECT_TRUE(complete_until_successful(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2);
	EXPECT_TRUE(complete_until_successful(objectdelete2));
	/**/
	BucketDelete bucketdelete1(BUCKET1);
	EXPECT_TRUE(complete_until_successful(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2);
	EXPECT_TRUE(complete_until_successful(bucketdelete2));
	/**/
	std::string command = "diff ";
	command += "dat/unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

TEST_F(ConvergenceTest, Complete) {
	Bucket BUCKET1("ConvergenceTestComplete1");
	Bucket BUCKET2("ConvergenceTestComplete2");
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_nonexistent(buckethead1));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_nonexistent(buckethead2));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketcreate2));
	/**/
	PathInput * input = new PathInput("dat/unittest.txt");
    ASSERT_FALSE(input == (PathInput *)0);
	Size inputsize = size(*input);
    ASSERT_FALSE(inputsize < 0);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < 10); ++ii) {
		logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("dat/unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, multiplex);
	EXPECT_TRUE(complete_until_successful(objecthead1));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objectcopy));
	/**/
	ObjectHead objecthead2(OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objecthead2));
	/**/
	PathOutput * output2 = new PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < 10); ++ii) {
		if (objectget2.isRetryable()) {
			logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
			output2 = new PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			logger.debug("WAITING %s@%d\n", __FILE__, __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, multiplex);
	EXPECT_TRUE(complete_until_successful(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objectdelete2));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketdelete2));
	/**/
	std::string command = "diff ";
	command += "dat/unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

TEST_F(ConvergenceTest, Service) {
	Bucket BUCKET1("ConvergenceTestService1");
	Bucket BUCKET2("ConvergenceTestService2");
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_nonexistent(buckethead1));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_nonexistent(buckethead2));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_successful(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	EXPECT_TRUE(service_until_successful(bucketcreate2));
	/**/
	PathInput * input = new PathInput("dat/unittest.txt");
    ASSERT_FALSE(input == (PathInput *)0);
	Size inputsize = size(*input);
    ASSERT_FALSE(inputsize < 0);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
		platform.yield(platform.frequency());
		input = new PathInput("dat/unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, multiplex);
	EXPECT_TRUE(service_until_successful(objecthead1));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objectcopy));
	/**/
	ObjectHead objecthead2(OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objecthead2));
	/**/
	PathOutput * output2 = new PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectget2.isRetryable()) {
			logger.debug("RETRYING %s@%d\n", __FILE__, __LINE__);
			output2 = new PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			logger.debug("WAITING %s@%d\n", __FILE__, __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, multiplex);
	EXPECT_TRUE(service_until_successful(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objectdelete2));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_successful(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	EXPECT_TRUE(service_until_successful(bucketdelete2));
	/**/
	std::string command = "diff ";
	command += "dat/unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

}
}
}

#endif
