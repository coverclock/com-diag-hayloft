/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_OBJECT_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_OBJECT_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/tostring.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture ObjectBaseTest;

TEST_F(ObjectBaseTest, Heap) {
	Bucket * bucket = new Bucket("ObjectBaseTestHeap");
	::com::diag::hayloft::Object * object = new ::com::diag::hayloft::Object("ObjectBaseTestHeapKey", *bucket);
	ASSERT_NE(object, (::com::diag::hayloft::Object*)0);
	EXPECT_TRUE((*object) == true);
	EXPECT_FALSE(object->isIdle());
	EXPECT_FALSE(object->isBusy());
	EXPECT_FALSE(object->isRetryable());
	EXPECT_EQ(object->getStatus(), ::S3StatusOK);
	EXPECT_NE(object->getKey(), (char *)0);
	delete object;
	delete bucket;
}

TEST_F(ObjectBaseTest, Stack) {
	Bucket bucket("ObjectBaseTestStack");
	::com::diag::hayloft::Object object("ObjectBaseTestStackKey", bucket);
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isIdle());
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	EXPECT_TRUE(::com::diag::hayloft::Object("ObjectBaseTestTemporaryKey", Bucket("ObjectBaseTestTemporary")) == true);
}

TEST_F(ObjectBaseTest, VirtualHostURI) {
	EndpointIreland endpoint;
	Session session(".virtualhost.diag.com", 0, endpoint);
	Credentials credentials;
	Region region;
	ProtocolSecure protocol;
	StyleVirtualHost style;
	Access access;
	Context context(credentials, region, protocol, style);
	Bucket bucket("virtualhosturi", context, session);
	::com::diag::hayloft::Object object("VirtualHost.txt", bucket);
	std::string uri;
	const char * str = tostring(object, uri);
	ASSERT_NE(str, (char *)0);
	printf("URI=\"%s\"\n", str);
	EXPECT_EQ(uri, "https://virtualhosturi.virtualhost.diag.com.s3-eu-west-1.amazonaws.com/VirtualHost.txt");
}

TEST_F(ObjectBaseTest, PathURI) {
	EndpointTokyo endpoint;
	Session session(".path.diag.com", 0, endpoint);
	Credentials credentials;
	Region region;
	ProtocolUnsecure protocol;
	StylePath style;
	Access access;
	Context context(credentials, region, protocol, style);
	Bucket bucket("pathuri", context, session);
	::com::diag::hayloft::Object object("Path.txt", bucket);
	std::string uri;
	const char * str = tostring(object, uri);
	ASSERT_NE(str, (char *)0);
	printf("URI=\"%s\"\n", str);
	EXPECT_EQ(uri, "http://s3-ap-northeast-1.amazonaws.com/pathuri.path.diag.com/Path.txt");
}

TEST_F(ObjectBaseTest, CopyConstructor) {
	EndpointTokyo endpoint;
	Session session(".copy.diag.com", 0, endpoint);
	Credentials credentials;
	RegionTokyo region;
	ProtocolUnsecure protocol;
	StylePath style;
	AccessPublicRead access;
	Context context(credentials, region, protocol, style, access);
	Bucket bucket("Bucket", context, session);
	::com::diag::hayloft::Object source("CopyConstructor", bucket);
	EXPECT_EQ(source.getHandle(), (Handle*)0);
	EXPECT_TRUE(source == true);
	EXPECT_FALSE(source.isIdle());
	EXPECT_FALSE(source.isBusy());
	EXPECT_FALSE(source.isRetryable());
	EXPECT_EQ(source.getStatus(), ::S3StatusOK);
	EXPECT_EQ(std::strcmp(source.getKey(), "CopyConstructor"), 0);
	EXPECT_EQ(std::strcmp(source.getCanonical(), "bucket.copy.diag.com"), 0);
	EXPECT_EQ(source.getStyle(), ::S3UriStylePath);
	EXPECT_NE(source.getId(), (char *)0);
	EXPECT_NE(source.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(source.getEndpoint(), Endpoint::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(source.getProtocol(), ::S3ProtocolHTTP);
	Multiplex multiplex;
	::com::diag::hayloft::Object sink(source, multiplex);
	EXPECT_NE(sink.getHandle(), (Handle*)0);
	EXPECT_TRUE(sink == true);
	EXPECT_FALSE(sink.isIdle());
	EXPECT_FALSE(sink.isBusy());
	EXPECT_FALSE(sink.isRetryable());
	EXPECT_EQ(sink.getStatus(), ::S3StatusOK);
	EXPECT_EQ(std::strcmp(source.getKey(), sink.getKey()), 0);
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

#endif
