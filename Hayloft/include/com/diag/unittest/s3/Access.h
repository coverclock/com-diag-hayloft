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
	Access list;
}

TEST_F(AccessTest, Default) {
	Access list;
	EXPECT_EQ(list.getAccess(), Access::DEFAULT);
}

TEST_F(AccessTest, Private) {
	AccessPrivate list;
	EXPECT_EQ(list.getAccess(), ::S3CannedAclPrivate);
}

TEST_F(AccessTest, PublicRead) {
	AccessPublicRead list;
	EXPECT_EQ(list.getAccess(), ::S3CannedAclPublicRead);
}

TEST_F(AccessTest, PublicReadWrite) {
	AccessPublicReadWrite list;
	EXPECT_EQ(list.getAccess(), ::S3CannedAclPublicReadWrite);
}

TEST_F(AccessTest, AuthenticatedRead) {
	AccessAuthenticatedRead list;
	EXPECT_EQ(list.getAccess(), ::S3CannedAclAuthenticatedRead);
}

static int cannedaccesscontrollistfunction(const Access & list = Access()) {
	return list.getAccess();
}

TEST_F(AccessTest, DefaultFunctionArgument) {
	EXPECT_EQ(cannedaccesscontrollistfunction(), Access::DEFAULT);
}

}
}
}
}

#endif
