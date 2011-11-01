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

	static const std::string ACCESSKEYIDPATH;
	static const std::string SECRETACCESSKEYPATH;

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

const std::string S3Test::ACCESSKEYIDPATH = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-access-key-id.txt";
const std::string S3Test::SECRETACCESSKEYPATH = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-secret-access-key.txt";

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

TEST_F(S3Test, SessionExplicit) {
	static const char * USER_AGENT_STR = "exp.hayloft.diag.com";
	static const char * HOST_NAME_STR = "s5.amazonaws.com";
	Session session(USER_AGENT_STR, S3_INIT_ALL, HOST_NAME_STR);
	EXPECT_TRUE(session.successful());
	ASSERT_NE(session.getUserAgent(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getUserAgent(), USER_AGENT_STR), 0);
	ASSERT_NE(session.getHostName(), (char *)0);
	EXPECT_EQ(std::strcmp(session.getHostName(), HOST_NAME_STR), 0);
}

/*******************************************************************************
 * S3 CREDENTIALS
 ******************************************************************************/

TEST_F(S3Test, CredentialsEnvironment) {
	const char * accesskeyidvalue = ::getenv(Credentials::ACCESS_KEY_ID_ENV());
	if (accesskeyidvalue == 0) {
		char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + 1];
		::com::diag::desperado::PathInput accesskeyidinput(S3Test::ACCESSKEYIDPATH.c_str(), "r");
		EXPECT_EQ(accesskeyidinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), accesskeyid, !0), 0);
	}
	ASSERT_NE(::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	const char * secretaccesskeyvalue = ::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	if (secretaccesskeyvalue == 0) {
		char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + 1];
		::com::diag::desperado::PathInput secretaccesskeyinput(S3Test::SECRETACCESSKEYPATH.c_str(), "r");
		EXPECT_EQ(secretaccesskeyinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secretaccesskey, !0), 0);
	}
	ASSERT_NE(::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	Credentials credentials;
	EXPECT_TRUE(credentials.successful());
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ::getenv(Credentials::ACCESS_KEY_ID_ENV())), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), ::getenv(Credentials::SECRET_ACCESS_KEY_ENV())), 0);
	if (accesskeyidvalue == 0) {
		EXPECT_EQ(::unsetenv(Credentials::ACCESS_KEY_ID_ENV()), 0);
	}
	if (secretaccesskeyvalue == 0) {
		EXPECT_EQ(::unsetenv(Credentials::SECRET_ACCESS_KEY_ENV()), 0);
	}
}

TEST_F(S3Test, CredentialsExplicit) {
	char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + 1];
	if (::getenv(Credentials::ACCESS_KEY_ID_ENV()) != 0) {
		std::strncpy(accesskeyid, ::getenv(Credentials::ACCESS_KEY_ID_ENV()), sizeof(accesskeyid));
		accesskeyid[sizeof(accesskeyid) - 1] = '\0';
	} else {
		::com::diag::desperado::PathInput accesskeyidinput(S3Test::ACCESSKEYIDPATH.c_str(), "r");
		EXPECT_EQ(accesskeyidinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
	}
	char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + 1];
	if (::getenv(Credentials::SECRET_ACCESS_KEY_ENV()) != 0) {
		std::strncpy(secretaccesskey, ::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), sizeof(secretaccesskey));
		secretaccesskey[sizeof(secretaccesskey) - 1] = '\0';
	} else {
		::com::diag::desperado::PathInput secretaccesskeyinput(S3Test::SECRETACCESSKEYPATH.c_str(), "r");
		EXPECT_EQ(secretaccesskeyinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
	}
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
