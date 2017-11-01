/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_CONVERGENCE_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_CONVERGENCE_

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

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture ConvergenceTest;

TEST_F(ConvergenceTest, Synchronous) {
	static const int LIMIT = 10;
	static const Logger::Level LEVEL = Logger::CONFIGURATION;
	Bucket BUCKET1("ConvergenceTestSynchronous1");
	Bucket BUCKET2("ConvergenceTestSynchronous2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	/**/
	BucketHead buckethead1(BUCKET1);
	EXPECT_TRUE(complete_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1);
	EXPECT_TRUE(complete_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1);
	EXPECT_TRUE(complete_until_successful(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2);
	EXPECT_TRUE(complete_until_successful(bucketcreate2, LEVEL));
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1);
	EXPECT_TRUE(complete_until_successful(objecthead1, LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2);
	EXPECT_TRUE(complete_until_successful(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2);
	EXPECT_TRUE(complete_until_successful(objecthead2, LEVEL));
	/**/
	::com::diag::grandote::PathOutput * output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1);
	EXPECT_TRUE(complete_until_successful(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2);
	EXPECT_TRUE(complete_until_successful(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1);
	EXPECT_TRUE(complete_until_successful(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2);
	EXPECT_TRUE(complete_until_successful(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

TEST_F(ConvergenceTest, Complete) {
	static const int LIMIT = 10;
	static const Logger::Level LEVEL = Logger::CONFIGURATION;
	Bucket BUCKET1("ConvergenceTestComplete1");
	Bucket BUCKET2("ConvergenceTestComplete2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketcreate2, LEVEL));
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < 10); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, multiplex);
	EXPECT_TRUE(complete_until_successful(objecthead1, LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objecthead2, LEVEL));
	/**/
	::com::diag::grandote::PathOutput * output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < 10); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, multiplex);
	EXPECT_TRUE(complete_until_successful(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2, multiplex);
	EXPECT_TRUE(complete_until_successful(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	EXPECT_TRUE(complete_until_successful(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

TEST_F(ConvergenceTest, Service) {
	Bucket BUCKET1("ConvergenceTestService1");
	Bucket BUCKET2("ConvergenceTestService2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	static const int LIMIT = 10;
	static const Logger::Level LEVEL = Logger::CONFIGURATION;
	Multiplex multiplex;
	/**/
	BucketHead buckethead1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_nonexistent(buckethead1, LEVEL));
	/**/
	BucketHead buckethead2(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_nonexistent(buckethead2, LEVEL));
	/**/
	BucketCreate bucketcreate1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_successful(bucketcreate1, LEVEL));
	/**/
	BucketCreate bucketcreate2(BUCKET2, multiplex);
	EXPECT_TRUE(service_until_successful(bucketcreate2, LEVEL));
	/**/
	::com::diag::grandote::PathInput * input = new ::com::diag::grandote::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, input, inputsize);
	for (int ii = 0; objectput1.isRetryable() && (ii < LIMIT); ++ii) {
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::grandote::PathInput("unittest.txt");
		inputsize = size(*input);
		EXPECT_TRUE(objectput1.reset(input, inputsize));
		EXPECT_TRUE(objectput1.start());
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectHead objecthead1(OBJECT1, multiplex);
	EXPECT_TRUE(service_until_successful(objecthead1, LEVEL));
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objectcopy, LEVEL));
	/**/
	ObjectHead objecthead2(OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objecthead2, LEVEL));
	/**/
	::com::diag::grandote::PathOutput * output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, output2);
	for (int ii = 0; (objectget2.isRetryable() || objectget2.isNonexistent()) &&  (ii < LIMIT); ++ii) {
		if (objectget2.isRetryable()) {
			printf("RETRYING %d\n", __LINE__);
			output2 = new ::com::diag::grandote::PathOutput(OBJECT2.getKey());
			EXPECT_TRUE(objectget2.reset(output2));
		} else if (objectget2.isNonexistent()) {
			printf("WAITING %d\n", __LINE__);
		}
		platform.yield(platform.frequency());
		EXPECT_TRUE(objectget2.start());
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, multiplex);
	EXPECT_TRUE(service_until_successful(objectdelete1, LEVEL));
	/**/
	ObjectDelete objectdelete2(OBJECT2, multiplex);
	EXPECT_TRUE(service_until_successful(objectdelete2, LEVEL));
	/**/
	BucketDelete bucketdelete1(BUCKET1, multiplex);
	EXPECT_TRUE(service_until_successful(bucketdelete1, LEVEL));
	/**/
	BucketDelete bucketdelete2(BUCKET2, multiplex);
	EXPECT_TRUE(service_until_successful(bucketdelete2, LEVEL));
	/**/
	std::string command = "diff ";
	command += "unittest.txt";
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
