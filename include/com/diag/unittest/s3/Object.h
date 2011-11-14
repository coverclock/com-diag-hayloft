/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_OBJECT
#define _H__COM_DIAG_UNITTEST_S3_OBJECT

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
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ObjectBaseTest;

TEST_F(ObjectBaseTest, Heap) {
	Session session;
	Bucket * bucket = new Bucket(session, "ObjectBaseTestHeap");
	Object * object = new Object(*bucket, "ObjectBaseTestHeapKey");
	ASSERT_NE(object, (Object*)0);
	EXPECT_TRUE((*object) == true);
	EXPECT_FALSE(object->isBusy());
	EXPECT_FALSE(object->isRetryable());
	EXPECT_EQ(object->getStatus(), ::S3StatusOK);
	EXPECT_NE(object->getKey(), (char *)0);
	delete object;
	delete bucket;
}

TEST_F(ObjectBaseTest, Stack) {
	Session session;
	Bucket bucket(session, "ObjectBaseTestStack");
	Object object(bucket, "ObjectBaseTestStackKey");
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	Session session;
	EXPECT_TRUE(Object(Bucket(session, "ObjectBaseTestTemporary"), "ObjectBaseTestTemporaryKey") == true);
}

}
}
}
}

#endif
