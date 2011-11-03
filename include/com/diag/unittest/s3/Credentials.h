/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_CREDENTIALS
#define _H_COM_DIAG_UNITTEST_S3_CREDENTIALS

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
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Credentials.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture CredentialsTest;

TEST_F(CredentialsTest, Environment) {
	if (std::getenv(Credentials::ACCESS_KEY_ID_ENV()) == 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), Environment::getAccessKeyId(), !0), 0);
	}
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	if (std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()) == 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), Environment::getSecretAccessKey(), !0), 0);
	}
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	Credentials credentials;
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), std::getenv(Credentials::ACCESS_KEY_ID_ENV())), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), std::getenv(Credentials::SECRET_ACCESS_KEY_ENV())), 0);
}

TEST_F(CredentialsTest, Explicit) {
	Credentials credentials(Environment::getAccessKeyId(), Environment::getSecretAccessKey());
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), Environment::getAccessKeyId()), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), Environment::getSecretAccessKey()), 0);
}

}
}
}
}

#endif
