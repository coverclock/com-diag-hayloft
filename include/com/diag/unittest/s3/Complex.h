/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_COMPLEX_
#define _H__COM_DIAG_UNITTEST_S3_COMPLEX_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
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

TEST_F(ComplexTest, Success) {
	Bucket BUCKET("ComplexTestSuccess");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	BucketCreate bucketcreate(BUCKET, complex);
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_TRUE(bucketcreate.isSuccessful());
	BucketDelete bucketdelete(BUCKET, complex);
	EXPECT_TRUE(complex.start(bucketdelete));
	EXPECT_TRUE(complex.wait(bucketdelete));
	EXPECT_TRUE(bucketdelete.isSuccessful());
}

struct BucketCreateProxy : public BucketCreate {
	int failures;
	Status failure;
	explicit BucketCreateProxy(const Bucket & bucket, const Plex & plex)
	: BucketCreate(bucket, plex)
	, failures(0)
	, failure(::S3StatusOK)
	{}
	virtual bool start(bool force = false) {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return BucketCreate::start(force);
		}
	}
};

TEST_F(ComplexTest, Recoverable) {
	Bucket BUCKET("ComplexTestRecoverable");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	BucketCreateProxy bucketcreate(BUCKET, complex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_TRUE(bucketcreate.isSuccessful());
	BucketDelete bucketdelete(BUCKET, complex);
	EXPECT_TRUE(complex.start(bucketdelete));
	EXPECT_TRUE(complex.wait(bucketdelete));
	EXPECT_TRUE(bucketdelete.isSuccessful());
}

TEST_F(ComplexTest, Recoverables) {
	Bucket BUCKET1("ComplexTestRecoverables1");
	Bucket BUCKET2("ComplexTestRecoverables2");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	BucketCreateProxy bucketcreate1(BUCKET1, complex);
	bucketcreate1.failures = 2;
	bucketcreate1.failure = ::S3StatusErrorInternalError;
	BucketCreateProxy bucketcreate2(BUCKET2, complex);
	bucketcreate2.failures = 3;
	bucketcreate2.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(complex.start(bucketcreate1));
	EXPECT_TRUE(complex.start(bucketcreate2));
	EXPECT_TRUE(complex.wait(bucketcreate1));
	EXPECT_TRUE(complex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	BucketDelete bucketdelete1(BUCKET1, complex);
	BucketDelete bucketdelete2(BUCKET2, complex);
	EXPECT_TRUE(complex.start(bucketdelete1));
	EXPECT_TRUE(complex.start(bucketdelete2));
	EXPECT_TRUE(complex.wait(bucketdelete1));
	EXPECT_TRUE(complex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	EXPECT_TRUE(bucketdelete2.isSuccessful());
}

TEST_F(ComplexTest, Unrecoverable) {
	Bucket BUCKET("ComplexTestUnrecoverable");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	BucketCreateProxy bucketcreate(BUCKET, complex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusAbortedByCallback;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct ComplexIrrecoverable : public Complex {
	explicit ComplexIrrecoverable() {
		RETRIES = 1;
	}
};

TEST_F(ComplexTest, Irrecoverable) {
	Bucket BUCKET("ComplexTestIrrecoverable");
	ComplexIrrecoverable complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	BucketCreateProxy bucketcreate(BUCKET, complex);
	bucketcreate.failures = 2;
	bucketcreate.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct ObjectPutProxy : public ObjectPut {
	int failures;
	Status failure;
	explicit ObjectPutProxy(const Object & object, const Plex & plex, Input * sourcep, Octets objectsize)
	: ObjectPut(object, plex, sourcep, objectsize)
	, failures(0)
	, failure(::S3StatusOK)
	{}
	virtual bool start(bool force = false) {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return ObjectPut::start(force);
		}
	}
};

struct ObjectGetProxy : public ObjectGet {
	int failures;
	Status failure;
	explicit ObjectGetProxy(const Object & object, const Plex & plex, Output * sinkp)
	: ObjectGet(object, plex, sinkp)
	, failures(0)
	, failure(::S3StatusOK)
	{}
	virtual bool start(bool force = false) {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return ObjectGet::start(force);
		}
	}
};

TEST_F(ComplexTest, Application) {
	static const int LIMIT = 10;
	Bucket BUCKET1("ComplexTestApplication1");
	Bucket BUCKET2("ComplexTestApplication2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getPending(), (Pending*)0);
	/**/
	BucketCreate bucketcreate1(BUCKET1, complex);
	EXPECT_TRUE(complex.start(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, complex);
	EXPECT_TRUE(complex.start(bucketcreate2));
	/**/
	EXPECT_TRUE(complex.wait(bucketcreate1));
	EXPECT_TRUE(complex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	/**/
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput("unittest.txt");
	Size inputsize = size(*input);
	ObjectPutProxy objectput1(OBJECT1, complex, input, inputsize);
	objectput1.failures = 2;
	objectput1.failure = ::S3StatusErrorInternalError;
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
	ObjectGetProxy objectget2(OBJECT2, complex, output2);
	objectget2.failures = 3;
	objectget2.failure = ::S3StatusErrorInternalError;
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
	/**/
	ObjectDelete objectdelete2(OBJECT2, complex);
	EXPECT_TRUE(complex.start(objectdelete2));
	/**/
	EXPECT_TRUE(complex.wait(objectdelete1));
	EXPECT_TRUE(complex.wait(objectdelete2));
	EXPECT_TRUE(objectdelete1.isSuccessful());
	EXPECT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketDelete bucketdelete1(BUCKET1, complex);
	EXPECT_TRUE(complex.start(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, complex);
	EXPECT_TRUE(complex.start(bucketdelete2));
	/**/
	EXPECT_TRUE(complex.wait(bucketdelete1));
	EXPECT_TRUE(complex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
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
