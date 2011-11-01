/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_UNITTEST_S3_H_
#define _COM_DIAG_UNITTEST_S3_H_

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
#include "com/diag/hayloft/S3.h"
#include "com/diag/hayloft/Packet.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/Print.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

/*******************************************************************************
 * S3 TEST FIXTURE
 ******************************************************************************/

class S3Test : public ::testing::Test {

public:

	static ::com::diag::desperado::Print printf;

	static const std::string accesskeyidpath;
	static const std::string secretaccesskeypath;

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

::com::diag::desperado::FileOutput S3Test::errput(::stderr);
::com::diag::desperado::LogOutput S3Test::logput(S3Test::errput);
::com::diag::desperado::Print S3Test::printf(S3Test::errput);

const std::string S3Test::accesskeyidpath = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-access-key-id.txt";
const std::string S3Test::secretaccesskeypath = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-secret-access-key.txt";

/*******************************************************************************
 * S3 SESSION
 ******************************************************************************/

TEST_F(S3Test, SessionDefaults) {
	Session session;
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), session.USER_AGENT_STR()), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), session.HOST_NAME_STR()), 0);
}

TEST_F(S3Test, SessionEnvironment) {
	static const char * USER_AGENT_STR = "environment.hayloft.diag.com";
	::setenv(Session::USER_AGENT_ENV(), USER_AGENT_STR, !0);
	::setenv(Session::HOST_NAME_ENV(), Session::HOST_NAME_STR(), !0);
	Session session;
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), session.HOST_NAME_STR()), 0);
}

TEST_F(S3Test, SessionExplicit) {
	static const char * USER_AGENT_STR = "explicit.hayloft.diag.com";
	Session session(USER_AGENT_STR, S3_INIT_ALL, Session::HOST_NAME_STR());
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), session.HOST_NAME_STR()), 0);
}

/*******************************************************************************
 * S3 CREDENTIALS
 ******************************************************************************/

TEST_F(S3Test, CredentialsEnvironment) {
	::com::diag::desperado::PathInput accesskeyidpathinput(S3Test::accesskeyidpath.c_str(), "r");
	char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + 1];
	EXPECT_EQ(accesskeyidpathinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
	::setenv(Credentials::ACCESS_KEY_ID_ENV(), accesskeyid, !0);
	::com::diag::desperado::PathInput secretaccesskeypathinput(S3Test::secretaccesskeypath.c_str(), "r");
	char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + 1];
	EXPECT_EQ(secretaccesskeypathinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
	::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secretaccesskey, !0);
	Credentials credentials;
	EXPECT_TRUE(credentials.successful());
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), accesskeyid), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), secretaccesskey), 0);
}

TEST_F(S3Test, CredentialsExplicit) {
	::com::diag::desperado::PathInput accesskeyidpathinput(S3Test::accesskeyidpath.c_str(), "r");
	char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + 1];
	EXPECT_EQ(accesskeyidpathinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
	::com::diag::desperado::PathInput secretaccesskeypathinput(secretaccesskeypath.c_str(), "r");
	char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + 1];
	EXPECT_EQ(secretaccesskeypathinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
	Credentials credentials(accesskeyid, secretaccesskey);
	EXPECT_TRUE(credentials.successful());
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), accesskeyid), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), secretaccesskey), 0);
}

/*******************************************************************************
 * S3
 ******************************************************************************/

}
}
}

#endif
