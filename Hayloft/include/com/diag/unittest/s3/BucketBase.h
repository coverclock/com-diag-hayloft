/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_BUCKET_
#define _H__COM_DIAG_UNITTEST_S3_BUCKET_

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
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/BucketValid.h"
#include "com/diag/hayloft/s3/tostring.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture BucketBaseTest;

TEST_F(BucketBaseTest, Heap) {
	Bucket * bucket = new Bucket("BucketBaseTestHeap");
	ASSERT_NE(bucket, (Bucket*)0);
	EXPECT_TRUE((*bucket) == true);
	EXPECT_FALSE(bucket->isIdle());
	EXPECT_FALSE(bucket->isBusy());
	EXPECT_FALSE(bucket->isRetryable());
	EXPECT_EQ(bucket->getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket->getName(), (char *)0);
	delete bucket;
}

TEST_F(BucketBaseTest, Stack) {
	Bucket bucket("BucketBaseTestStack");
	EXPECT_TRUE(bucket == true);
	EXPECT_FALSE(bucket.isIdle());
	EXPECT_FALSE(bucket.isBusy());
	EXPECT_FALSE(bucket.isRetryable());
	EXPECT_EQ(bucket.getStatus(), ::S3StatusOK);
	EXPECT_NE(bucket.getName(), (char *)0);
}

TEST_F(BucketBaseTest, Temporary) {
	EXPECT_TRUE(Bucket("BucketBaseTestTemporary") == true);
}

TEST_F(BucketBaseTest, VirtualHostURI) {
	EndpointIreland endpoint;
	Session session(".virtualhost.diag.com", 0, endpoint);
	Credentials credentials;
	Region region;
	ProtocolSecure protocol;
	StyleVirtualHost style;
	Access access;
	Context context(credentials, region, protocol, style);
	Bucket bucket("virtualhosturi", context, session);
	std::string uri;
	const char * str = tostring(bucket, uri);
	ASSERT_NE(str, (char *)0);
	printf("uri=\"%s\"\n", str);
	EXPECT_EQ(uri, "https://virtualhosturi.virtualhost.diag.com.s3-eu-west-1.amazonaws.com");
}

TEST_F(BucketBaseTest, PathURI) {
	EndpointTokyo endpoint;
	Session session(".path.diag.com", 0, endpoint);
	Credentials credentials;
	Region region;
	ProtocolUnsecure protocol;
	StylePath style;
	Access access;
	Context context(credentials, region, protocol, style);
	Bucket bucket("pathuri", context, session);
	std::string uri;
	const char * str = tostring(bucket, uri);
	ASSERT_NE(str, (char *)0);
	printf("uri=\"%s\"\n", str);
	EXPECT_EQ(uri, "http://s3-ap-northeast-1.amazonaws.com/pathuri.path.diag.com");
}

typedef Fixture BucketValidTest;

TEST_F(BucketValidTest, Heap) {
	BucketValid * valid = new BucketValid("BucketValidTestHeap");
	ASSERT_NE(valid, (BucketValid*)0);
	EXPECT_TRUE((*valid) == true);
	EXPECT_TRUE(valid->isSuccessful());
	delete valid;
}

TEST_F(BucketValidTest, Stack) {
	BucketValid valid("BucketValidTestStack");
	EXPECT_TRUE(valid == true);
	EXPECT_TRUE(valid.isSuccessful());
}

}
}
}
}

#endif
