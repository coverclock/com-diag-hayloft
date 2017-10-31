/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_PROPERTIES_
#define _H__COM_DIAG_UNITTEST_S3_PROPERTIES_

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
#include "com/diag/grandote/string.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture PropertiesTest;

TEST_F(PropertiesTest, Heap) {
	Properties * properties = new Properties;
	ASSERT_NE(properties->getType(), (char *)0);
	EXPECT_EQ(std::strcmp(properties->getType(), ""), 0);
	ASSERT_NE(properties->getChecksum(), (char *)0);
	EXPECT_EQ(std::strcmp(properties->getChecksum(), ""), 0);
	ASSERT_NE(properties->getControl(), (char *)0);
	EXPECT_EQ(std::strcmp(properties->getControl(), ""), 0);
	ASSERT_NE(properties->getFilename(), (char *)0);
	EXPECT_EQ(std::strcmp(properties->getFilename(), ""), 0);
	ASSERT_NE(properties->getEncoding(), (char *)0);
	EXPECT_EQ(std::strcmp(properties->getEncoding(), ""), 0);
	EXPECT_EQ(properties->getExpires(), -1);
	EXPECT_EQ(properties->getAccess(), Access::DEFAULT);
	EXPECT_TRUE((properties->getMetadata()).empty());
	delete properties;
}

TEST_F(PropertiesTest, Stack) {
	Properties properties;
	ASSERT_NE(properties.getType(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getType(), ""), 0);
	ASSERT_NE(properties.getChecksum(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getChecksum(), ""), 0);
	ASSERT_NE(properties.getControl(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getControl(), ""), 0);
	ASSERT_NE(properties.getFilename(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getFilename(), ""), 0);
	ASSERT_NE(properties.getEncoding(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getEncoding(), ""), 0);
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

TEST_F(PropertiesTest, Settors) {
	AccessPublicReadWrite access;
	Properties properties;
	properties.setType().setType("ContentType");
	ASSERT_NE(properties.getType(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getType(), "ContentType"), 0);
	properties.setChecksum().setChecksum("MD5");
	ASSERT_NE(properties.getChecksum(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getChecksum(), "MD5"), 0);
	properties.setControl().setControl("CacheControl");
	ASSERT_NE(properties.getControl(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getControl(), "CacheControl"), 0);
	properties.setFilename().setFilename("ContentDispositionFilename");
	ASSERT_NE(properties.getFilename(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getFilename(), "ContentDispositionFilename"), 0);
	properties.setEncoding().setEncoding("ContentEncoding");
	ASSERT_NE(properties.getEncoding(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getEncoding(), "ContentEncoding"), 0);
	properties.setExpires().setExpires(12345);
	EXPECT_EQ(properties.getExpires(), 12345);
	properties.setAccess().setAccess(access);
	EXPECT_EQ(properties.getAccess(), ::S3CannedAclPublicReadWrite);
	properties.setType().setChecksum().setControl().setFilename().setEncoding().setExpires().setAccess();
	ASSERT_NE(properties.getType(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getType(), ""), 0);
	ASSERT_NE(properties.getChecksum(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getChecksum(), ""), 0);
	ASSERT_NE(properties.getControl(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getControl(), ""), 0);
	ASSERT_NE(properties.getFilename(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getFilename(), ""), 0);
	ASSERT_NE(properties.getEncoding(), (char *)0);
	EXPECT_EQ(std::strcmp(properties.getEncoding(), ""), 0);
	EXPECT_EQ(properties.getExpires(), -1);
	EXPECT_EQ(properties.getAccess(), Access::DEFAULT);
}
}
}
}
}

#endif
