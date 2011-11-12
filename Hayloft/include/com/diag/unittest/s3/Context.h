/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_CONTEXT
#define _H__COM_DIAG_UNITTEST_S3_CONTEXT

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
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/desperado/string.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ContextTest;

TEST_F(ContextTest, Heap) {
	Context * context = new Context;
	ASSERT_NE(context, (Context*)0);
	delete context;
}

TEST_F(ContextTest, Stack) {
	Context context;
}

TEST_F(ContextTest, Implicit) {
	Context context;
	ASSERT_NE(context.getId(), (char *)0);
	EXPECT_EQ(std::strlen(context.getId()), Credentials::ACCESS_KEY_ID_LEN);
	ASSERT_NE(context.getSecret(), (char *)0);
	EXPECT_EQ(std::strlen(context.getSecret()), Credentials::SECRET_ACCESS_KEY_LEN);
	ASSERT_NE(context.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(context.getRegion(), Region::DEFAULT()), 0);
	EXPECT_EQ(context.getLength(), std::strlen(Region::DEFAULT()));
	EXPECT_EQ(context.getProtocol(), Protocol::DEFAULT);
	EXPECT_EQ(context.getStyle(), Style::DEFAULT);
	EXPECT_EQ(context.getAccess(), Access::DEFAULT);
}

TEST_F(ContextTest, Explicit) {
	Credentials credentials;
	RegionTokyo constraint;
	ProtocolUnsecure protocol;
	StyleVirtualHost style;
	AccessPublicRead list;
	Context context(credentials, constraint, protocol, style, list);
	ASSERT_NE(context.getId(), (char *)0);
	EXPECT_EQ(std::strlen(context.getId()), Credentials::ACCESS_KEY_ID_LEN);
	ASSERT_NE(context.getSecret(), (char *)0);
	EXPECT_EQ(std::strlen(context.getSecret()), Credentials::SECRET_ACCESS_KEY_LEN);
	ASSERT_NE(context.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(context.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(context.getLength(), std::strlen(Region::ASIA_PACIFIC_NORTHEAST_1()));
	EXPECT_EQ(context.getProtocol(), ::S3ProtocolHTTP);
	EXPECT_EQ(context.getStyle(), ::S3UriStyleVirtualHost);
	EXPECT_EQ(context.getAccess(), ::S3CannedAclPublicRead);
}

}
}
}
}

#endif
