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

#include <string>
#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/grandote/string.h"
#include "com/diag/hayloft/s3/S3.h"

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
	EXPECT_TRUE(bucket->isValid());
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
	EXPECT_TRUE(bucket.isValid());
}

TEST_F(BucketBaseTest, Temporary) {
	EXPECT_TRUE(Bucket("BucketBaseTestTemporary") == true);
}

TEST_F(BucketBaseTest, VirtualHostURI) {
	EndpointIreland endpoint;
	Session session(".virtualhost.diag.com", 0, endpoint);
	Credentials credentials;
	RegionIreland region;
	ProtocolSecure protocol;
	StyleVirtualHost style;
	AccessPublicRead access;
	Context context(credentials, region, protocol, style, access);
	Bucket bucket("virtualhosturi", context, session);
	std::string uri;
	const char * str = tostring(bucket, uri);
	ASSERT_NE(str, (char *)0);
	printf("URI=\"%s\"\n", str);
	EXPECT_EQ(uri, "https://virtualhosturi.virtualhost.diag.com.s3-eu-west-1.amazonaws.com");
}

TEST_F(BucketBaseTest, PathURI) {
	EndpointTokyo endpoint;
	Session session(".path.diag.com", 0, endpoint);
	Credentials credentials;
	RegionTokyo region;
	ProtocolUnsecure protocol;
	StylePath style;
	AccessPublicRead access;
	Context context(credentials, region, protocol, style, access);
	Bucket bucket("pathuri", context, session);
	std::string uri;
	const char * str = tostring(bucket, uri);
	ASSERT_NE(str, (char *)0);
	printf("URI=\"%s\"\n", str);
	EXPECT_EQ(uri, "http://s3-ap-northeast-1.amazonaws.com/pathuri.path.diag.com");
}

TEST_F(BucketBaseTest, CopyConstructor) {
	EndpointTokyo endpoint;
	Session session(".copy.diag.com", 0, endpoint);
	Credentials credentials;
	RegionTokyo region;
	ProtocolUnsecure protocol;
	StylePath style;
	AccessPublicRead access;
	Context context(credentials, region, protocol, style, access);
	Bucket source("CopyConstructor", context, session);
	ASSERT_EQ(source.getHandle(), (Handle*)0);
	EXPECT_TRUE(source == true);
	EXPECT_FALSE(source.isIdle());
	EXPECT_FALSE(source.isBusy());
	EXPECT_FALSE(source.isRetryable());
	EXPECT_EQ(source.getStatus(), ::S3StatusOK);
	EXPECT_EQ(std::strcmp(source.getName(), "CopyConstructor"), 0);
	EXPECT_EQ(std::strcmp(source.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(source.getAccess(), ::S3CannedAclPublicRead);
	EXPECT_EQ(std::strcmp(source.getCanonical(), "copyconstructor.copy.diag.com"), 0);
	EXPECT_EQ(source.getStyle(), ::S3UriStylePath);
	EXPECT_NE(source.getId(), (char *)0);
	EXPECT_NE(source.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(source.getEndpoint(), Endpoint::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(source.getProtocol(), ::S3ProtocolHTTP);
	Multiplex multiplex;
	Bucket sink(source, multiplex);
	ASSERT_NE(sink.getHandle(), (Handle*)0);
	EXPECT_TRUE(sink == true);
	EXPECT_FALSE(sink.isIdle());
	EXPECT_FALSE(sink.isBusy());
	EXPECT_FALSE(sink.isRetryable());
	EXPECT_EQ(sink.getStatus(), ::S3StatusOK);
	EXPECT_EQ(std::strcmp(source.getName(), sink.getName()), 0);
	EXPECT_EQ(std::strcmp(source.getRegion(), sink.getRegion()), 0);
	EXPECT_EQ(source.getAccess(), sink.getAccess());
	EXPECT_EQ(std::strcmp(source.getCanonical(), sink.getCanonical()), 0);
	EXPECT_EQ(source.getStyle(), sink.getStyle());
	EXPECT_NE(source.getId(), sink.getId());
	EXPECT_NE(source.getSecret(), sink.getSecret());
	EXPECT_EQ(std::strcmp(source.getEndpoint(), sink.getEndpoint()), 0);
	EXPECT_EQ(source.getProtocol(), sink.getProtocol());
}

}
}
}
}

#endif
