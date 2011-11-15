/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_CANNEDACCESSCONTROLLIST
#define _H_COM_DIAG_UNITTEST_S3_CANNEDACCESSCONTROLLIST

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Access.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture AccessTest;

TEST_F(AccessTest, Heap) {
	Access * pointer = new Access;
	EXPECT_NE(pointer, (Access*)0);
	delete pointer;
}

TEST_F(AccessTest, Stack) {
	Access access;
}

TEST_F(AccessTest, Default) {
	Access access;
	EXPECT_EQ(access.getAccess(), Access::DEFAULT);
}

TEST_F(AccessTest, Private) {
	AccessPrivate access;
	EXPECT_EQ(access.getAccess(), ::S3CannedAclPrivate);
}

TEST_F(AccessTest, PublicRead) {
	AccessPublicRead access;
	EXPECT_EQ(access.getAccess(), ::S3CannedAclPublicRead);
}

TEST_F(AccessTest, PublicReadWrite) {
	AccessPublicReadWrite access;
	EXPECT_EQ(access.getAccess(), ::S3CannedAclPublicReadWrite);
}

TEST_F(AccessTest, AuthenticatedRead) {
	AccessAuthenticatedRead access;
	EXPECT_EQ(access.getAccess(), ::S3CannedAclAuthenticatedRead);
}

static int cannedaccesscontrolaccessfunction(const Access & access = Access()) {
	return access.getAccess();
}

TEST_F(AccessTest, DefaultFunctionArgument) {
	EXPECT_EQ(cannedaccesscontrolaccessfunction(), Access::DEFAULT);
}

TEST_F(AccessTest, Settors) {
	Access access;
	access.setAccess().setAccess(::S3CannedAclPublicReadWrite);
	EXPECT_EQ(access.getAccess(), ::S3CannedAclPublicReadWrite);
}

}
}
}
}

#endif
