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

#include <cstdlib>
#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Session.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture SessionTest;

TEST_F(SessionTest, Defaults) {
	Session session;
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), session.USER_AGENT_STR()), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), session.HOST_NAME_STR()), 0);
}

TEST_F(SessionTest, Environment) {
	static const char * USER_AGENT_STR = "env.hayloft.diag.com";
	static const char * HOST_NAME_STR = "s4.amazonaws.com";
	::setenv(Session::USER_AGENT_ENV(), USER_AGENT_STR, !0);
	::setenv(Session::HOST_NAME_ENV(), HOST_NAME_STR, !0);
	Session session;
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_STR), 0);
}

TEST_F(SessionTest, Explicit) {
	static const char * USER_AGENT_STR = "exp.hayloft.diag.com";
	static const char * HOST_NAME_STR = "s5.amazonaws.com";
	Session session(USER_AGENT_STR, S3_INIT_ALL, HOST_NAME_STR);
	EXPECT_TRUE(session == true);
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_STR), 0);
}

}
}
}
}

#endif
