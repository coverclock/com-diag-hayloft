/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_PROPERTIES
#define _H__COM_DIAG_UNITTEST_S3_PROPERTIES

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
#include "com/diag/hayloft/s3/Properties.h"
#include "com/diag/desperado/string.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture PropertiesTest;

TEST_F(PropertiesTest, Heap) {
	Properties * properties = new Properties;
	EXPECT_EQ(properties->getType(), (char *)0);
	EXPECT_EQ(properties->getChecksum(), (char *)0);
	EXPECT_EQ(properties->getControl(), (char *)0);
	EXPECT_EQ(properties->getFilename(), (char *)0);
	EXPECT_EQ(properties->getEncoding(), (char *)0);
	EXPECT_EQ(properties->getExpires(), -1);
	EXPECT_EQ(properties->getAccess(), Access::DEFAULT);
	EXPECT_TRUE((properties->getMetadata()).empty());
	delete properties;
}

TEST_F(PropertiesTest, Stack) {
	Properties properties;
	EXPECT_EQ(properties.getType(), (char *)0);
	EXPECT_EQ(properties.getChecksum(), (char *)0);
	EXPECT_EQ(properties.getControl(), (char *)0);
	EXPECT_EQ(properties.getFilename(), (char *)0);
	EXPECT_EQ(properties.getEncoding(), (char *)0);
	EXPECT_EQ(properties.getExpires(), -1);
	EXPECT_EQ(properties.getAccess(), Access::DEFAULT);
	EXPECT_TRUE((properties.getMetadata()).empty());
}

TEST_F(PropertiesTest, Explicit) {
	AccessPublicReadWrite access;
	Properties properties("ContentType", "MD5", "CacheControl", "ContentDispositionFilename", "ContentEncoding", 12345, access);
	ASSERT_NE(properties.getType(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getType(), "ContentType"), 0);
	ASSERT_NE(properties.getChecksum(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getChecksum(), "MD5"), 0);
	ASSERT_NE(properties.getControl(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getControl(), "CacheControl"), 0);
	ASSERT_NE(properties.getFilename(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getFilename(), "ContentDispositionFilename"), 0);
	ASSERT_NE(properties.getEncoding(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getEncoding(), "ContentEncoding"), 0);
	EXPECT_EQ(properties.getExpires(), 12345);
	EXPECT_EQ(properties.getAccess(), ::S3CannedAclPublicReadWrite);
	EXPECT_TRUE((properties.getMetadata()).empty());
	properties.insert("Key1", "Value1");
	EXPECT_FALSE((properties.getMetadata()).empty());
	ASSERT_NE(properties.find("Key1"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key1"), "Value1"), 0);
	EXPECT_EQ(properties.find("Key2"), (char *)0);
	EXPECT_EQ(properties.find("Key3"), (char *)0);
	properties.insert("Key2", "Value2").insert("Key3", "Value3");
	ASSERT_NE(properties.find("Key1"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key1"), "Value1"), 0);
	ASSERT_NE(properties.find("Key2"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key2"), "Value2"), 0);
	ASSERT_NE(properties.find("Key3"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key3"), "Value3"), 0);
	properties.erase("Key1");
	EXPECT_EQ(properties.find("Key1"), (char *)0);
	ASSERT_NE(properties.find("Key2"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key2"), "Value2"), 0);
	ASSERT_NE(properties.find("Key3"), (char *)0);
	EXPECT_EQ(std::strcmp(properties.find("Key3"), "Value3"), 0);
	properties.erase("Key2").erase("Key3");
	EXPECT_EQ(properties.find("Key1"), (char *)0);
	EXPECT_EQ(properties.find("Key2"), (char *)0);
	EXPECT_EQ(properties.find("Key3"), (char *)0);
	EXPECT_TRUE((properties.getMetadata()).empty());
}

}
}
}
}

#endif
