/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_BUCKET
#define _H__COM_DIAG_UNITTEST_S3_BUCKET

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
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Bucket.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture BucketTest;

TEST_F(BucketTest, Name) {
	static const char NAME[] = "unittest";
	Bucket bucket(NAME);
	ASSERT_NE(bucket.getBucketName(), (char *)0);
	EXPECT_EQ(std::strcmp(bucket.getBucketName(), NAME), 0);
}

TEST_F(BucketTest, Valid) {
	static const char NAME[] = "unittest";
	Context context;
	Bucket bucket(NAME);
	EXPECT_TRUE(bucket.valid(context));
}

}
}
}
}

#endif
