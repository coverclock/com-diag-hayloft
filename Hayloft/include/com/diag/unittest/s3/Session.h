/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_SESSION
#define _H__COM_DIAG_UNITTEST_S3_SESSION

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
	ASSERT_EQ(session->getStatus(), ::S3StatusOK);
	ASSERT_NE(session->getUserAgent(), (char *)0);
	ASSERT_NE(session->getHostName(), (char *)0);
	delete session;
}

TEST_F(SessionTest, Stack) {
	Session session;
	EXPECT_TRUE(session == true);
	ASSERT_EQ(session.getStatus(), ::S3StatusOK);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	ASSERT_NE(session.getHostName(), (char *)0);
}

TEST_F(SessionTest, Environment) {
	const char * useragent = std::getenv(Session::USER_AGENT_ENV());
	const char * hostname = std::getenv(Session::HOST_NAME_ENV());
	static const char * USER_AGENT_VAL = "env.hayloft.diag.com";
	static const char * HOST_NAME_VAL = "s4.amazonaws.com";
	EXPECT_EQ(::setenv(Session::USER_AGENT_ENV(), USER_AGENT_VAL, !0), 0);
	EXPECT_EQ(::setenv(Session::HOST_NAME_ENV(), HOST_NAME_VAL, !0), 0);
	Session session;
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_VAL), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_VAL), 0);
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
	static const char * USER_AGENT_VAL = "exp.hayloft.diag.com";
	static const char * HOST_NAME_VAL = "s5.amazonaws.com";
	Session session(USER_AGENT_VAL, S3_INIT_ALL, HOST_NAME_VAL);
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_VAL), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_VAL), 0);
}

TEST_F(SessionTest, Canonicalization) {
	static const char * USER_AGENT_VAL = "Can.Hayloft.Diag.Com";
	Session session(USER_AGENT_VAL);
	EXPECT_TRUE(session == true);
	std::string name = session.canonicalize("AbCdEfGhIjKlMnOpQrStUvWxYzAbCdEfGhIjKlMnOpQrStUvWxYz");
	EXPECT_EQ(name, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopq.can.hayloft.diag.com");
    //               1234567890123456789012345678901234567890123456789012345678901234
	//                        1         2         3         4         5         6
}

}
}
}
}

#endif
