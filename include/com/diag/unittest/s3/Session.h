/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_SESSION_
#define _H__COM_DIAG_UNITTEST_S3_SESSION_

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
#include "com/diag/desperado/string.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Session.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture SessionTest;

TEST_F(SessionTest, Heap) {
	Session * session = new Session;
	ASSERT_NE(session, (Session*)0);
	EXPECT_TRUE(*session == true);
	EXPECT_EQ(session->getStatus(), ::S3StatusOK);
	EXPECT_NE(session->getBucketSuffix(), (char *)0);
	EXPECT_NE(session->getUserAgent(), (char *)0);
	EXPECT_NE(session->getHostName(), (char *)0);
	delete session;
}

TEST_F(SessionTest, Stack) {
	Session session;
	EXPECT_TRUE(session == true);
	EXPECT_EQ(session.getStatus(), ::S3StatusOK);
	EXPECT_NE(session.getBucketSuffix(), (char *)0);
	EXPECT_NE(session.getUserAgent(), (char *)0);
	EXPECT_NE(session.getHostName(), (char *)0);
}

TEST_F(SessionTest, Environment) {
	const char * bucketsuffix = std::getenv(Session::BUCKET_SUFFIX_ENV());
	const char * useragent = std::getenv(Session::USER_AGENT_ENV());
	const char * hostname = std::getenv(Session::HOST_NAME_ENV());
	static const char * BUCKET_SUFFIX_VAL = ".env.hayloft.diag.com";
	static const char * USER_AGENT_VAL = "env.hayloft.diag.com";
	static const char * HOST_NAME_VAL = "s4.amazonaws.com";
	EXPECT_EQ(::setenv(Session::BUCKET_SUFFIX_ENV(), BUCKET_SUFFIX_VAL, !0), 0);
	EXPECT_EQ(::setenv(Session::USER_AGENT_ENV(), USER_AGENT_VAL, !0), 0);
	EXPECT_EQ(::setenv(Session::HOST_NAME_ENV(), HOST_NAME_VAL, !0), 0);
	Session session;
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getBucketSuffix(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getBucketSuffix(), BUCKET_SUFFIX_VAL), 0);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_VAL), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_VAL), 0);
	if (bucketsuffix != 0) {
		EXPECT_EQ(::setenv(Session::BUCKET_SUFFIX_ENV(), bucketsuffix, !0), 0);
		ASSERT_NE(std::getenv(Session::BUCKET_SUFFIX_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Session::BUCKET_SUFFIX_ENV()), bucketsuffix), 0);
	} else {
		EXPECT_EQ(::unsetenv(Session::USER_AGENT_ENV()), 0);
		EXPECT_EQ(std::getenv(Session::USER_AGENT_ENV()), (char *)0);
	}
	if (useragent != 0) {
		EXPECT_EQ(::setenv(Session::USER_AGENT_ENV(), useragent, !0), 0);
		ASSERT_NE(std::getenv(Session::USER_AGENT_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Session::USER_AGENT_ENV()), useragent), 0);
	} else {
		EXPECT_EQ(::unsetenv(Session::USER_AGENT_ENV()), 0);
		EXPECT_EQ(std::getenv(Session::USER_AGENT_ENV()), (char *)0);
	}
	if (hostname != 0) {
		EXPECT_EQ(::setenv(Session::HOST_NAME_ENV(), hostname, !0), 0);
		ASSERT_NE(std::getenv(Session::HOST_NAME_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Session::HOST_NAME_ENV()), hostname), 0);
	} else {
		EXPECT_EQ(::unsetenv(Session::HOST_NAME_ENV()), 0);
		EXPECT_EQ(std::getenv(Session::HOST_NAME_ENV()), (char *)0);
	}
}

TEST_F(SessionTest, Explicit) {
	static const char * BUCKET_SUFFIX_VAL = ".exp.hayloft.diag.com";
	static const char * USER_AGENT_VAL = "exp.hayloft.diag.com";
	static const char * HOST_NAME_VAL = "s5.amazonaws.com";
	Session session(BUCKET_SUFFIX_VAL, USER_AGENT_VAL, S3_INIT_ALL, HOST_NAME_VAL);
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getBucketSuffix(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getBucketSuffix(), BUCKET_SUFFIX_VAL), 0);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_VAL), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_VAL), 0);
}

TEST_F(SessionTest, Canonicalization) {
	static const char * BUCKET_SUFFIX_VAL = ".Can.Hayloft.Diag.Com";
	Session session1(BUCKET_SUFFIX_VAL);
	EXPECT_TRUE(session1 == true);
	std::string name1 = session1.canonicalize("");
	EXPECT_EQ(name1, "");
	std::string name2 = session1.canonicalize("AbCdEfGhIjKlMnOpQrStUvWxYzAbCdEfGhIjKlMnOpQrStUvWxYz");
	EXPECT_EQ(name2, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz.can.hayloft.diag.com");
	Session session2("");
	EXPECT_TRUE(session2 == true);
	std::string name3 = session2.canonicalize("AbCdEfGhIjKlMnOpQrStUvWxYzAbCdEfGhIjKlMnOpQrStUvWxYzAbCdEfGhIjKl");
	EXPECT_EQ(name3, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijkl");
}

}
}
}
}

#endif
