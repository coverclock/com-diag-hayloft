/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_UNITTEST_S3_CREDENTIALS_H_
#define _COM_DIAG_UNITTEST_S3_CREDENTIALS_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdlib>
#include <string>
#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/desperado/PathInput.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture CredentialsTest;

static const std::string ACCESSKEYIDPATH = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-access-key-id.txt";
static const std::string SECRETACCESSKEYPATH = std::string(std::getenv("HOME")) + "/projects/hayloft/aws-s3-secret-access-key.txt";

TEST_F(CredentialsTest, Environment) {
	const char * accesskeyidvalue = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	if (accesskeyidvalue == 0) {
		char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + sizeof("\n")];
		::com::diag::desperado::PathInput accesskeyidinput(ACCESSKEYIDPATH.c_str(), "r");
		EXPECT_EQ(accesskeyidinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
		if (accesskeyid[sizeof(accesskeyid) - 2] == '\n') { accesskeyid[sizeof(accesskeyid) - 2] = '\0'; }
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), accesskeyid, !0), 0);
	}
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	const char * secretaccesskeyvalue = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	if (secretaccesskeyvalue == 0) {
		char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + sizeof("\n")];
		::com::diag::desperado::PathInput secretaccesskeyinput(SECRETACCESSKEYPATH.c_str(), "r");
		EXPECT_EQ(secretaccesskeyinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
		if (secretaccesskey[sizeof(secretaccesskey) - 2] == '\n') { secretaccesskey[sizeof(secretaccesskey) - 2] = '\0'; }
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secretaccesskey, !0), 0);
	}
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	Credentials credentials;
	EXPECT_TRUE(credentials.successful());
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), std::getenv(Credentials::ACCESS_KEY_ID_ENV())), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), std::getenv(Credentials::SECRET_ACCESS_KEY_ENV())), 0);
	if (accesskeyidvalue == 0) {
		EXPECT_EQ(::unsetenv(Credentials::ACCESS_KEY_ID_ENV()), 0);
	}
	if (secretaccesskeyvalue == 0) {
		EXPECT_EQ(::unsetenv(Credentials::SECRET_ACCESS_KEY_ENV()), 0);
	}
}

TEST_F(CredentialsTest, Explicit) {
	char accesskeyid[Credentials::ACCESS_KEY_ID_LEN + sizeof("\n")];
	if (std::getenv(Credentials::ACCESS_KEY_ID_ENV()) != 0) {
		std::strncpy(accesskeyid, std::getenv(Credentials::ACCESS_KEY_ID_ENV()), sizeof(accesskeyid));
		accesskeyid[sizeof(accesskeyid) - 1] = '\0';
	} else {
		::com::diag::desperado::PathInput accesskeyidinput(ACCESSKEYIDPATH.c_str(), "r");
		EXPECT_EQ(accesskeyidinput(accesskeyid, sizeof(accesskeyid)), sizeof(accesskeyid));
		if (accesskeyid[sizeof(accesskeyid) - 2] == '\n') { accesskeyid[sizeof(accesskeyid) - 2] = '\0'; }
	}
	char secretaccesskey[Credentials::SECRET_ACCESS_KEY_LEN + sizeof("\n")];
	if (std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()) != 0) {
		std::strncpy(secretaccesskey, std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), sizeof(secretaccesskey));
		secretaccesskey[sizeof(secretaccesskey) - 1] = '\0';
	} else {
		::com::diag::desperado::PathInput secretaccesskeyinput(SECRETACCESSKEYPATH.c_str(), "r");
		EXPECT_EQ(secretaccesskeyinput(secretaccesskey, sizeof(secretaccesskey)), sizeof(secretaccesskey));
		if (secretaccesskey[sizeof(secretaccesskey) - 2] == '\n') { secretaccesskey[sizeof(secretaccesskey) - 2] = '\0'; }
	}
	Credentials credentials(accesskeyid, secretaccesskey);
	EXPECT_TRUE(credentials.successful());
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), accesskeyid), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), secretaccesskey), 0);
}

}
}
}
}

#endif
