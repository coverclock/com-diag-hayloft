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
#include "com/diag/desperado/errno.h"

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
	EXPECT_NE(complex1.getHandle(), (Handle*)0);
	Complex complex2;
	EXPECT_TRUE(complex2 == true);
	EXPECT_EQ(complex2.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex2.getHandle(), (Handle*)0);
}

TEST_F(ComplexTest, Success) {
	Bucket BUCKET("ComplexTestSuccess");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreate bucketcreate(BUCKET, complex);
	EXPECT_FALSE(bucketcreate == true);
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_TRUE(bucketcreate == true);
	EXPECT_TRUE(bucketcreate.isSuccessful());
	BucketDelete bucketdelete(BUCKET, complex);
	EXPECT_FALSE(bucketdelete == true);
	EXPECT_TRUE(complex.start(bucketdelete));
	EXPECT_TRUE(complex.wait(bucketdelete));
	EXPECT_TRUE(bucketdelete == true);
	EXPECT_TRUE(bucketdelete.isSuccessful());
}

struct BucketCreateRecoverable : public BucketCreate {
	int failures;
	Status failure;
	explicit BucketCreateRecoverable(const Bucket & bucket, const Plex & plex)
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
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate(BUCKET, complex);
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
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate1(BUCKET1, complex);
	bucketcreate1.failures = 2;
	bucketcreate1.failure = ::S3StatusErrorInternalError;
	BucketCreateRecoverable bucketcreate2(BUCKET2, complex);
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
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate(BUCKET, complex);
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
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate(BUCKET, complex);
	bucketcreate.failures = 2;
	bucketcreate.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateUnretryable : public BucketCreateRecoverable {
	explicit BucketCreateUnretryable(const Bucket & bucket, const Plex & plex)
	: BucketCreateRecoverable(bucket, plex)
	{}
	virtual bool retryable(Status final, bool nonexistence = true) {
		return false;
	}
};

TEST_F(ComplexTest, Unretryable) {
	Bucket BUCKET("ComplexTestUnretryable");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateUnretryable bucketcreate(BUCKET, complex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusConnectionFailed;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateUnstartable : public BucketCreate {
	explicit BucketCreateUnstartable(const Bucket & bucket, const Plex & plex)
	: BucketCreate(bucket, plex)
	{}
	virtual bool start(bool force = false) {
		return false;
	}
};

TEST_F(ComplexTest, Unstartable) {
	Bucket BUCKET("ComplexTestUnstartable");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateUnstartable bucketcreate(BUCKET, complex);
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateUnresettable : public BucketCreateRecoverable {
	explicit BucketCreateUnresettable(const Bucket & bucket, const Plex & plex)
	: BucketCreateRecoverable(bucket, plex)
	{}
	virtual bool reset(bool force = false) {
		return false;
	}
};

TEST_F(ComplexTest, Unresettable) {
	Bucket BUCKET("ComplexTestUnresettable");
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getHandle(), (Handle*)0);
	BucketCreateUnresettable bucketcreate(BUCKET, complex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusConnectionFailed;
	EXPECT_TRUE(complex.start(bucketcreate));
	EXPECT_TRUE(complex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct ObjectPutApplication : public ObjectPut {
	int failures;
	Status failure;
	explicit ObjectPutApplication(const Object & object, const Plex & plex, Input * sourcep, Octets objectsize)
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

struct ObjectGetApplication : public ObjectGet {
	int failures;
	Status failure;
	explicit ObjectGetApplication(const Object & object, const Plex & plex, Output * sinkp)
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
	EXPECT_NE(complex.getHandle(), (Handle*)0);
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
	ObjectPutApplication objectput1(OBJECT1, complex, input, inputsize);
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
	ObjectGetApplication objectget2(OBJECT2, complex, output2);
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

struct ObjectPutFactory : public ObjectPut {
	const char * path;
	bool succeed;
	explicit ObjectPutFactory(const Object & object, const Plex & plex, const char * inputname)
	: ObjectPut(object, plex, new ::com::diag::desperado::PathInput(inputname), size(inputname))
	, path(inputname)
	, succeed(false)
	{}
	virtual bool reset(bool force = false) {
		return ObjectPut::reset(new ::com::diag::desperado::PathInput(path), size(path), force);
	}
	virtual int put(int bufferSize, void * buffer) {
		if (succeed) {
			return ObjectPut::put(bufferSize, buffer);
		} else if (consumed < (total / 2)) {
			return ObjectPut::put(bufferSize, buffer);
		} else {
			succeed = true;
			errno = EINVAL;
			return -1;
		}
	}
	virtual bool retryable(Status final, bool nonexistence = true) {
		return (final == ::S3StatusAbortedByCallback) ? true : ObjectPut::retryable(final, nonexistence);
	}
};

struct ObjectGetFactory : public ObjectGet {
	const char * path;
	bool succeed;
	explicit ObjectGetFactory(const Object & object, const Plex & plex, const char * outputname)
	: ObjectGet(object, plex, new ::com::diag::desperado::PathOutput(outputname))
	, path(outputname)
	, succeed(false)
	{}
	virtual bool reset(bool force = false) {
		return ObjectGet::reset(new ::com::diag::desperado::PathOutput(path), 0, 0, force);
	}
	virtual int get(int bufferSize, const void * buffer) {
		if (succeed) {
			return ObjectGet::get(bufferSize, buffer);
		} else if (produced < (total / 2)) {
			return ObjectGet::get(bufferSize, buffer);;
		} else {
			succeed = true;
			errno = EINVAL;
			return -1;
		}
	}
	virtual bool retryable(Status final, bool nonexistence = true) {
		return (final == ::S3StatusAbortedByCallback) ? true : ObjectGet::retryable(final, nonexistence);
	}
};

TEST_F(ComplexTest, Factory) {
	Bucket BUCKET1("ComplexTestFactory1");
	Bucket BUCKET2("ComplexTestFactory2");
	Object OBJECT1("Object1.txt", BUCKET1);
	Object OBJECT2("Object2.txt", BUCKET2);
	Complex complex;
	EXPECT_TRUE(complex == true);
	EXPECT_EQ(complex.getStatus(), ::S3StatusOK);
	EXPECT_NE(complex.getHandle(), (Handle*)0);
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
	ObjectPutFactory objectput1(OBJECT1, complex, "unittest.txt");
	EXPECT_TRUE(complex.start(objectput1));
	EXPECT_TRUE(complex.wait(objectput1));
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, complex);
	EXPECT_TRUE(complex.start(objectcopy));
	EXPECT_TRUE(complex.wait(objectcopy));
	EXPECT_TRUE(objectcopy.isSuccessful());
	/**/
	ObjectGetFactory objectget2(OBJECT2, complex, OBJECT2.getKey());
	EXPECT_TRUE(complex.start(objectget2));
	EXPECT_TRUE(complex.wait(objectget2));
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
