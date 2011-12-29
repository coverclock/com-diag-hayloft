/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_COMPLEX_
#define _H__COM_DIAG_UNITTEST_S3_COMPLEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
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

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ComplexTest;

TEST_F(ComplexTest, Stack) {
	Complex complex1;
	EXPECT_TRUE(complex1 == true);
	EXPECT_EQ(complex1.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex1.getPending(), (Pending*)0);
	Complex complex2;
	EXPECT_TRUE(complex2 == true);
	EXPECT_EQ(complex2.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex2.getPending(), (Pending*)0);
}

TEST_F(ComplexTest, Foo) {
	static const int LIMIT = 10;
	Bucket BUCKET1("ComplexTestService1");
	Bucket BUCKET2("ComplexTestService2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	/**/
	BucketCreate bucketcreate1(BUCKET1, complex);
	EXPECT_TRUE(complex.start(bucketcreate1));
	EXPECT_TRUE(complex.wait(bucketcreate1));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	/**/
	BucketCreate bucketcreate2(BUCKET2, complex);
	EXPECT_TRUE(complex.start(bucketcreate2));
	EXPECT_TRUE(complex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPut objectput1(OBJECT1, complex, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(complex.start(objectput1));
		EXPECT_TRUE(complex.wait(objectput1));
		if (objectput1.isSuccessful()) {
			break;
		}
		input = new ::com::diag::desperado::PathInput("unittest.txt");
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, complex);
	EXPECT_TRUE(complex.start(objectcopy));
	EXPECT_TRUE(complex.wait(objectcopy));
	EXPECT_TRUE(objectcopy.isSuccessful());
	/**/
	::com::diag::desperado::PathOutput * output2 = new ::com::diag::desperado::PathOutput(OBJECT2.getKey());
	ObjectGet objectget2(OBJECT2, complex, output2);
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(complex.start(objectget2));
		EXPECT_TRUE(complex.wait(objectget2));
		if (objectget2.isSuccessful()) {
			break;
		}
		output2 = new ::com::diag::desperado::PathOutput(OBJECT2.getKey());
		objectget2.reset(output2);
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, complex);
	EXPECT_TRUE(complex.start(objectdelete1));
	EXPECT_TRUE(complex.wait(objectdelete1));
	EXPECT_TRUE(objectdelete1.isSuccessful());
	/**/
	ObjectDelete objectdelete2(OBJECT2, complex);
	EXPECT_TRUE(complex.start(objectdelete2));
	EXPECT_TRUE(complex.wait(objectdelete2));
	EXPECT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketDelete bucketdelete1(BUCKET1, complex);
	EXPECT_TRUE(complex.start(bucketdelete1));
	EXPECT_TRUE(complex.wait(bucketdelete1));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	/**/
	BucketDelete bucketdelete2(BUCKET2, complex);
	EXPECT_TRUE(complex.start(bucketdelete2));
	EXPECT_TRUE(complex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete2.isSuccessful());
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
}

#endif
