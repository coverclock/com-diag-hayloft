/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_CREDENTIALS_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_CREDENTIALS_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/Credentials.h"
#include "com/diag/grandote/Parameter.h"
#include "com/diag/grandote/DataInput.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/hayloft/Debug.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture CredentialsTest;

TEST_F(CredentialsTest, Heap) {
	static const char ID[] = "AAAAAAAAAABBBBBBBBBB";
	static const char SECRET[] = "CCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFF";
	Credentials * pointer = new Credentials(ID, SECRET);
	EXPECT_NE(pointer, (Credentials*)0);
	delete pointer;
}

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
	} else {
		EXPECT_EQ(::unsetenv(Credentials::ACCESS_KEY_ID_ENV()), 0);
		EXPECT_EQ(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
	}
	if (secret != 0) {
		EXPECT_EQ(::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret, !0), 0);
		ASSERT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), secret), 0);
	} else {
		EXPECT_EQ(::unsetenv(Credentials::SECRET_ACCESS_KEY_ENV()), 0);
		EXPECT_EQ(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
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
	DataInput idin(ID);
	DataInput secretin(SECRET);
	Parameter idparm(idin);
	Parameter secretparm(secretin);
	Credentials credentials(idparm, secretparm);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
}

TEST_F(CredentialsTest, InputTaken) {
	static const char ID[] = "SSSSSSSSSSTTTTTTTTTT";
	static const char SECRET[] = "UUUUUUUUUUVVVVVVVVVVWWWWWWWWWWXXXXXXXXXX";
	DataInput * idinp = new DataInput(ID);
	DataInput * secretinp = new DataInput(SECRET);
	Parameter idparm(idinp);
	Parameter secretparm(secretinp);
	Credentials credentials(idparm, secretparm);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
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

TEST_F(CredentialsTest, Settors) {
	static const char ID[] = "AAAAAAAAAABBBBBBBBBB";
	static const char SECRET[] = "CCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFF";
	Credentials credentials;
	credentials.setId().setId(ID);
	credentials.setSecret().setSecret(SECRET);
	EXPECT_TRUE(credentials == true);
	ASSERT_NE(credentials.getId(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getId(), ID), 0);
	ASSERT_NE(credentials.getSecret(), (char *)0);
	EXPECT_EQ(std::strcmp(credentials.getSecret(), SECRET), 0);
	credentials.setId().setSecret();
}

TEST_F(CredentialsTest, Final) {
    ASSERT_NE(Credentials::ACCESS_KEY_ID_ENV(), (char *)0);
    EXPECT_NE(std::getenv(Credentials::ACCESS_KEY_ID_ENV()), (char *)0);
    if (Debugging) {
        fprintf(stderr, "%s=\"%s\"\n", Credentials::ACCESS_KEY_ID_ENV(),  std::getenv(Credentials::ACCESS_KEY_ID_ENV()));
    }
    ASSERT_NE(Credentials::SECRET_ACCESS_KEY_ENV(), (char *)0);
    EXPECT_NE(std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()), (char *)0);
    if (Debugging) {
        fprintf(stderr, "%s=\"%s\"\n", Credentials::SECRET_ACCESS_KEY_ENV(), std::getenv(Credentials::SECRET_ACCESS_KEY_ENV()));
    }
}

}
}
}

#endif
