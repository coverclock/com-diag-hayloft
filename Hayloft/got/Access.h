/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_CANNEDACCESSCONTROLLIST_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_CANNEDACCESSCONTROLLIST_

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
#include "com/diag/hayloft/Access.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

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
	EXPECT_EQ(access.setAccess().getAccess(), Access::DEFAULT);
}

}
}
}

#endif
