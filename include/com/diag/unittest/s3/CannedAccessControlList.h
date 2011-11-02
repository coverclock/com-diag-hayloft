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

#include <cstring>
#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture CannedAccessControlListTest;

TEST_F(CannedAccessControlListTest, Default) {
	CannedAccessControlList acl;
	EXPECT_EQ(acl.getCannedAccessControlList(), ::S3CannedAclPrivate);
}

TEST_F(CannedAccessControlListTest, Private) {
	CannedAccessControlListPrivate acl;
	EXPECT_EQ(acl.getCannedAccessControlList(), ::S3CannedAclPrivate);
}

TEST_F(CannedAccessControlListTest, PublicRead) {
	CannedAccessControlListPublicRead acl;
	EXPECT_EQ(acl.getCannedAccessControlList(), ::S3CannedAclPublicRead);
}

TEST_F(CannedAccessControlListTest, PublicReadWrite) {
	CannedAccessControlListPublicReadWrite acl;
	EXPECT_EQ(acl.getCannedAccessControlList(), ::S3CannedAclPublicReadWrite);
}

TEST_F(CannedAccessControlListTest, AuthenticatedRead) {
	CannedAccessControlListAuthenticatedRead acl;
	EXPECT_EQ(acl.getCannedAccessControlList(), ::S3CannedAclAuthenticatedRead);
}

}
}
}
}

#endif
