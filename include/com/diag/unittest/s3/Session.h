/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_UNITTEST_S3_SESSION_H_
#define _COM_DIAG_UNITTEST_S3_SESSION_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include "gtest/gtest.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

class SessionTest : public ::testing::Test {

public:

	static ::com::diag::desperado::Print printf;
	static ::com::diag::desperado::Dump dump;

private:

	static ::com::diag::desperado::FileOutput errput;
	static ::com::diag::desperado::LogOutput logput;

	::com::diag::desperado::Output * output;
	Logger::Mask mask;

protected:

	virtual void SetUp() {
		output = &(Logger::instance().getOutput());
		mask = Logger::instance().getMask();
		Logger::instance()
			.setOutput(logput)
	    	.enable(Logger::FINEST)
	    	.enable(Logger::FINER)
	    	.enable(Logger::FINE)
	    	.enable(Logger::TRACE)
	    	.enable(Logger::DEBUG)
	    	.enable(Logger::INFORMATION)
	    	.enable(Logger::CONFIGURATION)
	    	.enable(Logger::NOTICE)
	    	.enable(Logger::WARNING)
	    	.enable(Logger::ERROR)
	    	.enable(Logger::SEVERE)
	    	.enable(Logger::CRITICAL)
	    	.enable(Logger::ALERT)
	    	.enable(Logger::FATAL)
	    	.enable(Logger::EMERGENCY)
	    	.enable(Logger::PRINT);
	}

	virtual void TearDown() {
		Logger::instance().setMask(mask);
		Logger::instance().setOutput(*output);
	}

};

::com::diag::desperado::FileOutput SessionTest::errput(::stderr);
::com::diag::desperado::LogOutput SessionTest::logput(SessionTest::errput);
::com::diag::desperado::Print SessionTest::printf(SessionTest::errput);
::com::diag::desperado::Dump SessionTest::dump(SessionTest::errput);


TEST_F(SessionTest, Defaults) {
	Session session;
	EXPECT_TRUE(session.successful());
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
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_STR), 0);
}

TEST_F(SessionTest, Explicit) {
	static const char * USER_AGENT_STR = "exp.hayloft.diag.com";
	static const char * HOST_NAME_STR = "s5.amazonaws.com";
	Session session(USER_AGENT_STR, S3_INIT_ALL, HOST_NAME_STR);
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_STR), 0);
}

}
}
}

#endif
