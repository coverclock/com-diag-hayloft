/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_GRANT_
#define _H__COM_DIAG_UNITTEST_S3_GRANT_

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
#include "com/diag/hayloft/s3/Grant.h"
#include "com/diag/hayloft/s3/show.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture GrantBaseTest;

TEST_F(GrantBaseTest, Sanity) {
	Grant grant1;
	EXPECT_EQ(grant1.getAccessControlList().size(), 0);
	EXPECT_EQ(grant1.import(::S3GranteeTypeCanonicalUser, ::S3PermissionFullControl, "0000000000000000000000000000000000000000", "coverclock"), 1);
	EXPECT_EQ(grant1.import(::S3GranteeTypeAmazonCustomerByEmail, ::S3PermissionRead, "coverclock@diag.com"), 1);
	EXPECT_EQ(grant1.import(::S3GranteeTypeAllUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(grant1.import(::S3GranteeTypeAllAwsUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(grant1.getAccessControlList().size(), 4);
	show(grant1);
	Grant grant2(grant1);
	EXPECT_EQ(grant2.getAccessControlList().size(), 4);
	show(grant2);
	Grant grant3;
	EXPECT_EQ(grant3.getAccessControlList().size(), 0);
	EXPECT_EQ(grant3.import(grant1), 4);
	show(grant3);
	EXPECT_EQ(grant3.getAccessControlList().size(), 4);
	int count = -1;
	::S3AclGrant * grants = grant1.generate(count);
	EXPECT_EQ(count, 4);
	ASSERT_NE(grants, (::S3AclGrant*)0);
	Grant grant4;
	EXPECT_EQ(grant4.getAccessControlList().size(), 0);
	EXPECT_EQ(grant4.import(count, grants), 4);
	EXPECT_EQ(grant4.getAccessControlList().size(), 4);
	show(grant4);
	delete grants;
}


}
}
}
}

#endif
