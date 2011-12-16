/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_CONTEXT_
#define _H__COM_DIAG_UNITTEST_S3_CONTEXT_

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
#include "com/diag/hayloft/s3/S3.h"

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
	EXPECT_EQ(context.getProtocol(), Protocol::DEFAULT);
	EXPECT_EQ(context.getStyle(), Style::DEFAULT);
	EXPECT_EQ(context.getAccess(), Access::DEFAULT);
}

TEST_F(ContextTest, Explicit) {
	Credentials credentials;
	RegionTokyo constraint;
	ProtocolUnsecure protocol;
	StyleVirtualHost style;
	AccessPublicRead access;
	Context context(credentials, constraint, protocol, style, access);
	ASSERT_NE(context.getId(), (char *)0);
	EXPECT_EQ(std::strlen(context.getId()), Credentials::ACCESS_KEY_ID_LEN);
	ASSERT_NE(context.getSecret(), (char *)0);
	EXPECT_EQ(std::strlen(context.getSecret()), Credentials::SECRET_ACCESS_KEY_LEN);
	ASSERT_NE(context.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(context.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(context.getProtocol(), ::S3ProtocolHTTP);
	EXPECT_EQ(context.getStyle(), ::S3UriStyleVirtualHost);
	EXPECT_EQ(context.getAccess(), ::S3CannedAclPublicRead);
}

TEST_F(ContextTest, Settors) {
	Credentials credentials;
	RegionTokyo constraint;
	ProtocolUnsecure protocol;
	StylePath style;
	AccessPublicRead access;
	Context context;
	context.setCredentials().setCredentials(credentials);
	ASSERT_NE(context.getId(), (char *)0);
	EXPECT_EQ(std::strlen(context.getId()), Credentials::ACCESS_KEY_ID_LEN);
	ASSERT_NE(context.getSecret(), (char *)0);
	EXPECT_EQ(std::strlen(context.getSecret()), Credentials::SECRET_ACCESS_KEY_LEN);
	context.setRegion().setRegion(constraint);
	ASSERT_NE(context.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(context.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	context.setProtocol().setProtocol(protocol);
	EXPECT_EQ(context.getProtocol(), ::S3ProtocolHTTP);
	context.setStyle().setStyle(style);
	EXPECT_EQ(context.getStyle(), ::S3UriStylePath);
	context.setAccess().setAccess(access);
	EXPECT_EQ(context.getAccess(), ::S3CannedAclPublicRead);
	context.setCredentials().setRegion().setProtocol().setStyle().setAccess();
	ASSERT_NE(context.getId(), (char *)0);
	EXPECT_EQ(std::strlen(context.getId()), Credentials::ACCESS_KEY_ID_LEN);
	ASSERT_NE(context.getSecret(), (char *)0);
	EXPECT_EQ(std::strlen(context.getSecret()), Credentials::SECRET_ACCESS_KEY_LEN);
	ASSERT_NE(context.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(context.getRegion(), Region::DEFAULT()), 0);
	EXPECT_EQ(context.getProtocol(), Protocol::DEFAULT);
	EXPECT_EQ(context.getStyle(), Style::DEFAULT);
	EXPECT_EQ(context.getAccess(), Access::DEFAULT);
}

}
}
}
}

#endif
