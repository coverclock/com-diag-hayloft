/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_COMPLEX_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_COMPLEX_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "Environment.h"
#include "Fixture.h"
#include "com/diag/hayloft/Complex.h"
#include "com/diag/hayloft/LifeCycle.h"
#include "com/diag/hayloft/ObjectPut.h"
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
#include "com/diag/grandote/errno.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef ::com::diag::hayloft::Object HayloftObject; // Resolve Object ambiguity.

typedef Fixture ComplexTest;

TEST_F(ComplexTest, Stack) {
	Complex komplex1;
	EXPECT_TRUE(komplex1 == true);
	EXPECT_EQ(komplex1.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex1.getHandle(), (Handle*)0);
	Complex komplex2;
	EXPECT_TRUE(komplex2 == true);
	EXPECT_EQ(komplex2.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex2.getHandle(), (Handle*)0);
}

TEST_F(ComplexTest, Success) {
	Bucket BUCKET("ComplexTestSuccess");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreate bucketcreate(BUCKET, komplex);
	EXPECT_FALSE(bucketcreate == true);
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_TRUE(bucketcreate == true);
	EXPECT_TRUE(bucketcreate.isSuccessful());
	BucketDelete bucketdelete(BUCKET, komplex);
	EXPECT_FALSE(bucketdelete == true);
	EXPECT_TRUE(komplex.start(bucketdelete));
	EXPECT_TRUE(komplex.wait(bucketdelete));
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
	virtual bool start() {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return BucketCreate::start();
		}
	}
};

TEST_F(ComplexTest, Recoverable) {
	Bucket BUCKET("ComplexTestRecoverable");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate(BUCKET, komplex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_TRUE(bucketcreate.isSuccessful());
	BucketDelete bucketdelete(BUCKET, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete));
	EXPECT_TRUE(komplex.wait(bucketdelete));
	EXPECT_TRUE(bucketdelete.isSuccessful());
}

TEST_F(ComplexTest, Recoverables) {
	Bucket BUCKET1("ComplexTestRecoverables1");
	Bucket BUCKET2("ComplexTestRecoverables2");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate1(BUCKET1, komplex);
	bucketcreate1.failures = 2;
	bucketcreate1.failure = ::S3StatusErrorInternalError;
	BucketCreateRecoverable bucketcreate2(BUCKET2, komplex);
	bucketcreate2.failures = 3;
	bucketcreate2.failure = ::S3StatusErrorInternalError;
	EXPECT_TRUE(komplex.start(bucketcreate1));
	EXPECT_TRUE(komplex.start(bucketcreate2));
	EXPECT_TRUE(komplex.wait(bucketcreate1));
	EXPECT_TRUE(komplex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	BucketDelete bucketdelete1(BUCKET1, komplex);
	BucketDelete bucketdelete2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete1));
	EXPECT_TRUE(komplex.start(bucketdelete2));
	EXPECT_TRUE(komplex.wait(bucketdelete1));
	EXPECT_TRUE(komplex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	EXPECT_TRUE(bucketdelete2.isSuccessful());
}

TEST_F(ComplexTest, Unrecoverable) {
	Bucket BUCKET("ComplexTestUnrecoverable");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateRecoverable bucketcreate(BUCKET, komplex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusAbortedByCallback;
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateIrrecoverable : public BucketCreateRecoverable {
	int limit;
	explicit BucketCreateIrrecoverable(const Bucket & bucket, const Plex & plex)
	: BucketCreateRecoverable(bucket, plex)
	, limit(0)
	{}
	virtual bool startable(Handle * candidate) {
		bool result = BucketCreateRecoverable::startable(candidate);
		if (result) { retries = limit; }
		return result;
	}
};

TEST_F(ComplexTest, Irrecoverable) {
	Bucket BUCKET("ComplexTestIrrecoverable");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateIrrecoverable bucketcreate(BUCKET, komplex);
	bucketcreate.failures = 2;
	bucketcreate.failure = ::S3StatusErrorInternalError;
	bucketcreate.limit = 1;
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
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
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateUnretryable bucketcreate(BUCKET, komplex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusConnectionFailed;
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateUnstartable : public BucketCreate {
	explicit BucketCreateUnstartable(const Bucket & bucket, const Plex & plex)
	: BucketCreate(bucket, plex)
	{}
	virtual bool start() {
		return false;
	}
};

TEST_F(ComplexTest, Unstartable) {
	Bucket BUCKET("ComplexTestUnstartable");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateUnstartable bucketcreate(BUCKET, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct BucketCreateUnresettable : public BucketCreateRecoverable {
	explicit BucketCreateUnresettable(const Bucket & bucket, const Plex & plex)
	: BucketCreateRecoverable(bucket, plex)
	{}
	virtual bool reset() {
		return false;
	}
};

TEST_F(ComplexTest, Unresettable) {
	Bucket BUCKET("ComplexTestUnresettable");
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	BucketCreateUnresettable bucketcreate(BUCKET, komplex);
	bucketcreate.failures = 1;
	bucketcreate.failure = ::S3StatusConnectionFailed;
	EXPECT_TRUE(komplex.start(bucketcreate));
	EXPECT_TRUE(komplex.wait(bucketcreate));
	EXPECT_FALSE(bucketcreate.isSuccessful());
}

struct ObjectPutApplication : public ObjectPut {
	int failures;
	Status failure;
	explicit ObjectPutApplication(const HayloftObject & object, const Plex & plex, Input * sourcep, Octets objectsize)
	: ObjectPut(object, plex, sourcep, objectsize)
	, failures(0)
	, failure(::S3StatusOK)
	{}
	virtual bool start() {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return ObjectPut::start();
		}
	}
};

struct ObjectGetApplication : public ObjectGet {
	int failures;
	Status failure;
	explicit ObjectGetApplication(const HayloftObject & object, const Plex & plex, Output * sinkp)
	: ObjectGet(object, plex, sinkp)
	, failures(0)
	, failure(::S3StatusOK)
	{}
	virtual bool start() {
		if (failures > 0) {
			--failures;
			::S3ErrorDetails errorDetails = { 0 };
			responseCompleteCallback(failure, &errorDetails, this);
			return true;
		} else {
			return ObjectGet::start();
		}
	}
};

TEST_F(ComplexTest, Application) {
	static const int LIMIT = Fixture::limit(10);
	Bucket BUCKET1("ComplexTestApplication1");
	Bucket BUCKET2("ComplexTestApplication2");
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	/**/
	BucketCreate bucketcreate1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate2));
	/**/
	EXPECT_TRUE(komplex.wait(bucketcreate1));
	EXPECT_TRUE(komplex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	/**/
	PathInput * input = new PathInput("dat/unittest.txt");
    ASSERT_FALSE(input == (PathInput *)0);
	Size inputsize = size(*input);
    ASSERT_FALSE(inputsize < 0);
	ObjectPutApplication objectput1(OBJECT1, komplex, input, inputsize);
	objectput1.failures = 2;
	objectput1.failure = ::S3StatusErrorInternalError;
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(komplex.start(objectput1));
		EXPECT_TRUE(komplex.wait(objectput1));
		if (objectput1.isSuccessful()) {
			break;
		}
		input = new PathInput("dat/unittest.txt");
		inputsize = size(*input);
		objectput1.reset(input, inputsize);
	}
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectcopy));
	EXPECT_TRUE(komplex.wait(objectcopy));
	EXPECT_TRUE(objectcopy.isSuccessful());
	/**/
	PathOutput * output2 = new PathOutput(OBJECT2.getKey());
	ObjectGetApplication objectget2(OBJECT2, komplex, output2);
	objectget2.failures = 3;
	objectget2.failure = ::S3StatusErrorInternalError;
	for (int ii = 0; ii < LIMIT; ++ii) {
		EXPECT_TRUE(komplex.start(objectget2));
		EXPECT_TRUE(komplex.wait(objectget2));
		if (objectget2.isSuccessful()) {
			break;
		}
		output2 = new PathOutput(OBJECT2.getKey());
		objectget2.reset(output2);
	}
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, komplex);
	EXPECT_TRUE(komplex.start(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectdelete2));
	/**/
	EXPECT_TRUE(komplex.wait(objectdelete1));
	EXPECT_TRUE(komplex.wait(objectdelete2));
	EXPECT_TRUE(objectdelete1.isSuccessful());
	EXPECT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketDelete bucketdelete1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete2));
	/**/
	EXPECT_TRUE(komplex.wait(bucketdelete1));
	EXPECT_TRUE(komplex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	EXPECT_TRUE(bucketdelete2.isSuccessful());
	/**/
	std::string command = "diff ";
	command += "dat/unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

struct ObjectPutFactory : public ObjectPut {
	const char * path;
	bool succeed;
	explicit ObjectPutFactory(const HayloftObject & object, const Plex & plex, const char * inputname)
	: ObjectPut(object, plex, new PathInput(inputname), size(inputname))
	, path(inputname)
	, succeed(false)
	{}
	virtual bool reset() {
		return ObjectPut::reset(new PathInput(path), size(path));
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
	explicit ObjectGetFactory(const HayloftObject & object, const Plex & plex, const char * outputname)
	: ObjectGet(object, plex, new PathOutput(outputname))
	, path(outputname)
	, succeed(false)
	{}
	virtual bool reset() {
		return ObjectGet::reset(new PathOutput(path), 0, 0);
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
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	/**/
	BucketCreate bucketcreate1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate2));
	/**/
	EXPECT_TRUE(komplex.wait(bucketcreate1));
	EXPECT_TRUE(komplex.wait(bucketcreate2));
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	/**/
	ObjectPutFactory objectput1(OBJECT1, komplex, "dat/unittest.txt");
	EXPECT_TRUE(komplex.start(objectput1));
	EXPECT_TRUE(komplex.wait(objectput1));
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectcopy));
	EXPECT_TRUE(komplex.wait(objectcopy));
	EXPECT_TRUE(objectcopy.isSuccessful());
	/**/
	ObjectGetFactory objectget2(OBJECT2, komplex, OBJECT2.getKey());
	EXPECT_TRUE(komplex.start(objectget2));
	EXPECT_TRUE(komplex.wait(objectget2));
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, komplex);
	EXPECT_TRUE(komplex.start(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectdelete2));
	/**/
	EXPECT_TRUE(komplex.wait(objectdelete1));
	EXPECT_TRUE(komplex.wait(objectdelete2));
	EXPECT_TRUE(objectdelete1.isSuccessful());
	EXPECT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketDelete bucketdelete1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete2));
	/**/
	EXPECT_TRUE(komplex.wait(bucketdelete1));
	EXPECT_TRUE(komplex.wait(bucketdelete2));
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	EXPECT_TRUE(bucketdelete2.isSuccessful());
	/**/
	std::string command = "diff ";
	command += "dat/unittest.txt";
	command += " ";
	command += OBJECT2.getKey();
	EXPECT_EQ(std::system(command.c_str()), 0);
	/**/
	EXPECT_EQ(::unlink(OBJECT2.getKey()), 0);
}

static void komplextestpoll(Action & action) {
	while (action != true) { Thread::yield(); }
}

TEST_F(ComplexTest, Polled) {
	Bucket BUCKET1("ComplexTestPolled1");
	Bucket BUCKET2("ComplexTestPolled2");
	HayloftObject OBJECT1("Object1.txt", BUCKET1);
	HayloftObject OBJECT2("Object2.txt", BUCKET2);
	Complex komplex;
	EXPECT_TRUE(komplex == true);
	EXPECT_EQ(komplex.getStatus(), ::S3StatusOK);
	EXPECT_NE(komplex.getHandle(), (Handle*)0);
	/**/
	BucketCreate bucketcreate1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate1));
	/**/
	BucketCreate bucketcreate2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketcreate2));
	/**/
	komplextestpoll(bucketcreate1);
	komplextestpoll(bucketcreate2);
	EXPECT_TRUE(bucketcreate1.isSuccessful());
	EXPECT_TRUE(bucketcreate2.isSuccessful());
	/**/
	ObjectPutFactory objectput1(OBJECT1, komplex, "dat/unittest.txt");
	EXPECT_TRUE(komplex.start(objectput1));
	komplextestpoll(objectput1);
	EXPECT_TRUE(objectput1.isSuccessful());
	/**/
	ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectcopy));
	komplextestpoll(objectcopy);
	EXPECT_TRUE(objectcopy.isSuccessful());
	/**/
	ObjectGetFactory objectget2(OBJECT2, komplex, OBJECT2.getKey());
	EXPECT_TRUE(komplex.start(objectget2));
	komplextestpoll(objectget2);
	EXPECT_TRUE(objectget2.isSuccessful());
	/**/
	ObjectDelete objectdelete1(OBJECT1, komplex);
	EXPECT_TRUE(komplex.start(objectdelete1));
	/**/
	ObjectDelete objectdelete2(OBJECT2, komplex);
	EXPECT_TRUE(komplex.start(objectdelete2));
	/**/
	komplextestpoll(objectdelete1);
	komplextestpoll(objectdelete2);
	EXPECT_TRUE(objectdelete1.isSuccessful());
	EXPECT_TRUE(objectdelete2.isSuccessful());
	/**/
	BucketDelete bucketdelete1(BUCKET1, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete1));
	/**/
	BucketDelete bucketdelete2(BUCKET2, komplex);
	EXPECT_TRUE(komplex.start(bucketdelete2));
	/**/
	komplextestpoll(bucketdelete1);
	komplextestpoll(bucketdelete2);
	EXPECT_TRUE(bucketdelete1.isSuccessful());
	EXPECT_TRUE(bucketdelete2.isSuccessful());
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
