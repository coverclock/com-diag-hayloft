/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_OBJECT_
#define _H__COM_DIAG_UNITTEST_S3_OBJECT_

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
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/tostring.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ObjectBaseTest;

TEST_F(ObjectBaseTest, Heap) {
	Bucket * bucket = new Bucket("ObjectBaseTestHeap");
	Object * object = new Object("ObjectBaseTestHeapKey", *bucket);
	ASSERT_NE(object, (Object*)0);
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
	Object object("ObjectBaseTestStackKey", bucket);
	EXPECT_TRUE(object == true);
	EXPECT_FALSE(object.isIdle());
	EXPECT_FALSE(object.isBusy());
	EXPECT_FALSE(object.isRetryable());
	EXPECT_EQ(object.getStatus(), ::S3StatusOK);
	EXPECT_NE(object.getKey(), (char *)0);
}

TEST_F(ObjectBaseTest, Temporary) {
	EXPECT_TRUE(Object("ObjectBaseTestTemporaryKey", Bucket("ObjectBaseTestTemporary")) == true);
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
	Object object("VirtualHost.txt", bucket);
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
	Object object("Path.txt", bucket);
	std::string uri;
	const char * str = tostring(object, uri);
	ASSERT_NE(str, (char *)0);
	printf("URI=\"%s\"\n", str);
	EXPECT_EQ(uri, "http://s3-ap-northeast-1.amazonaws.com/pathuri.path.diag.com/Path.txt");
}

}
}
}
}

#endif
