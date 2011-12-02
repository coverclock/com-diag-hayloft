/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_GRANT_
#define _H__COM_DIAG_UNITTEST_S3_GRANT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/GrantGet.h"
#include "com/diag/hayloft/s3/GrantXml.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/size.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/convergence.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture GrantTest;

TEST_F(GrantTest, GetSynchronous) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetSynchronous";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	BucketCreate bucketcreate(BUCKET);
	ASSERT_TRUE(complete_until_successful(bucketcreate));
	GrantGet grantgetbucket(bucketcreate);
	ASSERT_TRUE(complete_until_successful(grantgetbucket));
	show(grantgetbucket, LEVEL);
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, input, inputsize);
	for (int ii = 0; ii < LIMIT; ++ii) {
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput, bucketcreate);
	ASSERT_TRUE(complete_until_successful(grantgetobject));
	show(grantgetobject, LEVEL);
	ObjectDelete objectdelete(OBJECT, bucketcreate);
	ASSERT_TRUE(complete_until_successful(objectdelete));
	BucketDelete bucketdelete(BUCKET);
	ASSERT_TRUE(complete_until_successful(bucketdelete));
}

TEST_F(GrantTest, GetAsynchronous) {
	static const int LIMIT = 10;
	static const char BUCKET[] = "GrantTestGetAsynchronous";
	static const char OBJECT[] = "Object.txt";
	static const Logger::Level LEVEL = Logger::PRINT;
	Multiplex multiplex;
	BucketCreate bucketcreate(BUCKET, multiplex);
	ASSERT_TRUE(complete_until_successful(bucketcreate));
	GrantGet grantgetbucket(bucketcreate, multiplex);
	ASSERT_TRUE(complete_until_successful(grantgetbucket));
	show(grantgetbucket, LEVEL);
	::com::diag::desperado::PathInput * input = new ::com::diag::desperado::PathInput(__FILE__);
	Size inputsize = size(*input);
	ObjectPut objectput(OBJECT, bucketcreate, multiplex, input, inputsize);
	objectput.start();
	for (int ii = 0; ii < LIMIT; ++ii) {
		multiplex.complete();
		if (!objectput.isRetryable()) { break; }
		printf("RETRYING %d\n", __LINE__);
		platform.yield(platform.frequency());
		input = new ::com::diag::desperado::PathInput(__FILE__);
		inputsize = size(*input);
		objectput.reset(input, inputsize);
		objectput.start();
	}
	ASSERT_TRUE(objectput.isSuccessful());
	GrantGet grantgetobject(objectput, bucketcreate, multiplex);
	ASSERT_TRUE(complete_until_successful(grantgetobject));
	show(grantgetobject, LEVEL);
	ObjectDelete objectdelete(OBJECT, bucketcreate, multiplex);
	ASSERT_TRUE(complete_until_successful(objectdelete));
	BucketDelete bucketdelete(BUCKET, multiplex);
	ASSERT_TRUE(complete_until_successful(bucketdelete));
}

}
}
}
}

#endif
