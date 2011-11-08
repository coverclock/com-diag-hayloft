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

TEST_F(BucketTest, Heap) {
	Bucket * pointer = new Bucket("BucketTestHeap");
	EXPECT_NE(pointer, (Bucket*)0);
	delete pointer;
}

TEST_F(BucketTest, Stack) {
	Bucket bucket("BucketTestStack");
}

}
}
}
}

#endif
