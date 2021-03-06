/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_GRANTBASE_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_GRANTBASE_

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
#include "com/diag/hayloft/Grant.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

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
	delete [] grants;
}


}
}
}

#endif
