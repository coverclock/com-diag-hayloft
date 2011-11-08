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
#include "com/diag/desperado/DataInput.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture CredentialsTest;

TEST_F(CredentialsTest, Explicit) {
	static const char ID[] = "AAAAAAAAAABBBBBBBBBB";
	static const char SECRET[] = "CCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFF";
	Credentials credentials(ID, SECRET);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, ExplicitShort) {
	static const char ID[] = "AAAAAAAAAABBBBBBBBB";
	static const char SECRET[] = "CCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFF";
	Credentials credentials(ID, SECRET);
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, ExplicitLong) {
	static const char ID[] = "AAAAAAAAAABBBBBBBBBBZ";
	static const char SECRET[] = "CCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFFX";
	Credentials credentials(ID, SECRET);
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, Environment) {
	static const char ID[] = "GGGGGGGGGGHHHHHHHHHH";
	static const char SECRET[] = "IIIIIIIIIIJJJJJJJJJJKKKKKKKKKKLLLLLLLLLL";
	const char * id = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), ID, !0), 0);
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), ID), 0);
	const char * secret = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), SECRET, !0), 0);
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), SECRET), 0);
	Credentials credentials;
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	if (id != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id), 0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	}
}

TEST_F(CredentialsTest, EnvironmentShort) {
	static const char ID[] = "GGGGGGGGGGHHHHHHHHH";
	static const char SECRET[] = "IIIIIIIIIIJJJJJJJJJJKKKKKKKKKKLLLLLLLLL";
	const char * id = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), ID, !0), 0);
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), ID), 0);
	const char * secret = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), SECRET, !0), 0);
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), SECRET), 0);
	Credentials credentials;
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	if (id != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id), 0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	}
}

TEST_F(CredentialsTest, EnvironmentLong) {
	static const char ID[] = "GGGGGGGGGGHHHHHHHHHHZ";
	static const char SECRET[] = "IIIIIIIIIIJJJJJJJJJJKKKKKKKKKKLLLLLLLLLLZ";
	const char * id = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), ID, !0), 0);
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), ID), 0);
	const char * secret = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), SECRET, !0), 0);
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), SECRET), 0);
	Credentials credentials;
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	if (id != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id), 0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	}
}

TEST_F(CredentialsTest, ExplicitOverridesEnvironment) {
	static const char ID2[] = "gggggggggghhhhhhhhhh";
	static const char SECRET2[] = "iiiiiiiiiijjjjjjjjjjkkkkkkkkkkllllllllll";
	const char * id = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), ID2, !0), 0);
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), ID2), 0);
	const char * secret = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), SECRET2, !0), 0);
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), SECRET2), 0);
	static const char ID[] = "GGGGGGGGGGHHHHHHHHHH";
	static const char SECRET[] = "IIIIIIIIIIJJJJJJJJJJKKKKKKKKKKLLLLLLLLLL";
	Credentials credentials(ID, SECRET);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	if (id != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id), 0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	}
}

TEST_F(CredentialsTest, Input) {
	static const char ID[] = "SSSSSSSSSSTTTTTTTTTT";
	static const char SECRET[] = "UUUUUUUUUUVVVVVVVVVVWWWWWWWWWWXXXXXXXXXX";
	::com::diag::desperado::DataInput idin(ID);
	::com::diag::desperado::DataInput secretin(SECRET);
	Credentials credentials(idin, secretin);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, InputShort) {
	static const char ID[] = "SSSSSSSSSSTTTTTTTTT";
	static const char SECRET[] = "UUUUUUUUUUVVVVVVVVVVWWWWWWWWWWXXXXXXXXX";
	::com::diag::desperado::DataInput idin(ID);
	::com::diag::desperado::DataInput secretin(SECRET);
	Credentials credentials(idin, secretin);
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, InputLong) {
	static const char ID[] = "SSSSSSSSSSTTTTTTTTTTZ";
	static const char SECRET[] = "UUUUUUUUUUVVVVVVVVVVWWWWWWWWWWXXXXXXXXXXZ";
	::com::diag::desperado::DataInput idin(ID);
	::com::diag::desperado::DataInput secretin(SECRET);
	Credentials credentials(idin, secretin);
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, InputOverridesEnvironment) {
	static const char ID2[] = "gggggggggghhhhhhhhhh";
	static const char SECRET2[] = "iiiiiiiiiijjjjjjjjjjkkkkkkkkkkllllllllll";
	const char * id2 = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), ID2, !0), 0);
	ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), ID2), 0);
	const char * secret2 = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), SECRET2, !0), 0);
	ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), SECRET2), 0);
	static const char ID[] = "SSSSSSSSSSTTTTTTTTTT";
	static const char SECRET[] = "UUUUUUUUUUVVVVVVVVVVWWWWWWWWWWXXXXXXXXXX";
	::com::diag::desperado::DataInput idin(ID);
	::com::diag::desperado::DataInput secretin(SECRET);
	Credentials credentials(idin, secretin);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	if (id2 != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id2, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id2), 0);
	}
	if (secret2 != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret2, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret2), 0);
	}
}

TEST_F(CredentialsTest, Missing) {
	const char * id = std::getenv(Credentials::ACCESS_KEY_ID_ENV());
	EXPECT_EQ(::unsetenv(Credentials::ACCESS_KEY_ID_ENV()), 0);
	EXPECT_EQ(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	const char * secret = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV());
	EXPECT_EQ(::unsetenv(Credentials::SECRET_ACCESS_KEY_ENV()), 0);
	EXPECT_EQ(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
	Credentials credentials;
	EXPECT_TRUE(credentials == false);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ""), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), ""), 0);
	if (id != 0) {
		EXPECT_EQ(::setenv(Credentials::ACCESS_KEY_ID_ENV(), id, !0), 0);
		ASSERT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), id), 0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	}
}

}
}
}
}

#endif
