/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_LOGBASE_
#define _H__COM_DIAG_UNITTEST_S3_LOGBASE_

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
#include "com/diag/hayloft/s3/Log.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/desperado/string.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture LogBaseTest;

TEST_F(LogBaseTest, NoPrefix) {
	Bucket bucket("Bucket");
	Bucket log("Log");
	Log log1(bucket, log);
	ASSERT_NE(log1.getCanonical(), (char *)0);
	ASSERT_NE(bucket.getCanonical(), (char *)0);
	EXPECT_EQ(std::strcmp(log1.getCanonical(), bucket.getCanonical()), 0);
	ASSERT_NE(log1.getTarget(), (char *)0);
	ASSERT_NE(log.getCanonical(), (char *)0);
	EXPECT_EQ(std::strcmp(log1.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(log1.getPrefix(), (char *)0);
	EXPECT_EQ(*log1.getPrefix(), '\0');
	EXPECT_EQ(log1.getAccessControlList().size(), 0);
	EXPECT_EQ(log1.import(::S3GranteeTypeCanonicalUser, ::S3PermissionFullControl, "0000000000000000000000000000000000000000", "coverclock"), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAmazonCustomerByEmail, ::S3PermissionRead, "coverclock@diag.com"), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAllUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAllAwsUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(log1.getAccessControlList().size(), 4);
	show(log1);
	Log log2(bucket, log, 0, log1);
	EXPECT_EQ(log2.getAccessControlList().size(), 4);
	show(log2);
	Log log3(bucket, log);
	EXPECT_EQ(log3.getAccessControlList().size(), 0);
	EXPECT_EQ(log3.import(log1), 4);
	show(log3);
	EXPECT_EQ(log3.getAccessControlList().size(), 4);
	int count = -1;
	::S3AclGrant * grants = log1.generate(count);
	EXPECT_EQ(count, 4);
	ASSERT_NE(grants, (::S3AclGrant*)0);
	Log log4(bucket, log);
	EXPECT_EQ(log4.getAccessControlList().size(), 0);
	EXPECT_EQ(log4.import(count, grants), 4);
	EXPECT_EQ(log4.getAccessControlList().size(), 4);
	show(log4);
	delete [] grants;
}

TEST_F(LogBaseTest, Prefix) {
	static const char PREFIX[] = "Prefix";
	Bucket bucket("Bucket");
	Bucket log("Log");
	Log log1(bucket, log, PREFIX);
	ASSERT_NE(log1.getCanonical(), (char *)0);
	ASSERT_NE(bucket.getCanonical(), (char *)0);
	EXPECT_EQ(std::strcmp(log1.getCanonical(), bucket.getCanonical()), 0);
	ASSERT_NE(log1.getTarget(), (char *)0);
	ASSERT_NE(log.getCanonical(), (char *)0);
	EXPECT_EQ(std::strcmp(log1.getTarget(), log.getCanonical()), 0);
	ASSERT_NE(log1.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(log1.getPrefix(), PREFIX), 0);
	EXPECT_EQ(log1.getAccessControlList().size(), 0);
	EXPECT_EQ(log1.import(::S3GranteeTypeCanonicalUser, ::S3PermissionFullControl, "0000000000000000000000000000000000000000", "coverclock"), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAmazonCustomerByEmail, ::S3PermissionRead, "coverclock@diag.com"), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAllUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(log1.import(::S3GranteeTypeAllAwsUsers, ::S3PermissionRead), 1);
	EXPECT_EQ(log1.getAccessControlList().size(), 4);
	show(log1);
	Log log2(bucket, log, PREFIX, log1);
	EXPECT_EQ(log2.getAccessControlList().size(), 4);
	show(log2);
	Log log3(bucket, log, PREFIX);
	EXPECT_EQ(log3.getAccessControlList().size(), 0);
	EXPECT_EQ(log3.import(log1), 4);
	show(log3);
	EXPECT_EQ(log3.getAccessControlList().size(), 4);
	int count = -1;
	::S3AclGrant * grants = log1.generate(count);
	EXPECT_EQ(count, 4);
	ASSERT_NE(grants, (::S3AclGrant*)0);
	Log log4(bucket, log, PREFIX);
	EXPECT_EQ(log4.getAccessControlList().size(), 0);
	EXPECT_EQ(log4.import(count, grants), 4);
	EXPECT_EQ(log4.getAccessControlList().size(), 4);
	show(log4);
	delete [] grants;
}

}
}
}
}

#endif
