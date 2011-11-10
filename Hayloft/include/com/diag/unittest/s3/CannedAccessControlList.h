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
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture CannedAccessControlListTest;

TEST_F(CannedAccessControlListTest, Heap) {
	CannedAccessControlList * pointer = new CannedAccessControlList;
	EXPECT_NE(pointer, (CannedAccessControlList*)0);
	delete pointer;
}

TEST_F(CannedAccessControlListTest, Stack) {
	CannedAccessControlList list;
}

TEST_F(CannedAccessControlListTest, Default) {
	CannedAccessControlList list;
	EXPECT_EQ(list.getList(), CannedAccessControlList::DEFAULT);
}

TEST_F(CannedAccessControlListTest, Private) {
	CannedAccessControlListPrivate list;
	EXPECT_EQ(list.getList(), ::S3CannedAclPrivate);
}

TEST_F(CannedAccessControlListTest, PublicRead) {
	CannedAccessControlListPublicRead list;
	EXPECT_EQ(list.getList(), ::S3CannedAclPublicRead);
}

TEST_F(CannedAccessControlListTest, PublicReadWrite) {
	CannedAccessControlListPublicReadWrite list;
	EXPECT_EQ(list.getList(), ::S3CannedAclPublicReadWrite);
}

TEST_F(CannedAccessControlListTest, AuthenticatedRead) {
	CannedAccessControlListAuthenticatedRead list;
	EXPECT_EQ(list.getList(), ::S3CannedAclAuthenticatedRead);
}

static int cannedaccesscontrollistfunction(const CannedAccessControlList & list = CannedAccessControlList()) {
	return list.getList();
}

TEST_F(CannedAccessControlListTest, DefaultFunctionArgument) {
	EXPECT_EQ(cannedaccesscontrollistfunction(), CannedAccessControlList::DEFAULT);
}

}
}
}
}

#endif
