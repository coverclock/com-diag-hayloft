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
	const ::S3PutProperties * pp = properties->getProperties();
	ASSERT_NE(pp, (::S3PutProperties*)0);
	EXPECT_EQ(pp->contentType, (char *)0);
	EXPECT_EQ(pp->md5, (char *)0);
	EXPECT_EQ(pp->cacheControl, (char *)0);
	EXPECT_EQ(pp->contentDispositionFilename, (char *)0);
	EXPECT_EQ(pp->contentEncoding, (char *)0);
	EXPECT_EQ(pp->expires, -1);
	EXPECT_EQ(pp->cannedAcl, Access::DEFAULT);
	EXPECT_EQ(pp->metaDataCount, 0);
	EXPECT_EQ(pp->metaData, (::S3NameValue*)0);
	delete properties;
}

TEST_F(PropertiesTest, Stack) {
	Properties properties;
	const ::S3PutProperties * pp = properties.getProperties();
	ASSERT_NE(pp, (::S3PutProperties*)0);
	EXPECT_EQ(pp->contentType, (char *)0);
	EXPECT_EQ(pp->md5, (char *)0);
	EXPECT_EQ(pp->cacheControl, (char *)0);
	EXPECT_EQ(pp->contentDispositionFilename, (char *)0);
	EXPECT_EQ(pp->contentEncoding, (char *)0);
	EXPECT_EQ(pp->expires, -1);
	EXPECT_EQ(pp->cannedAcl, Access::DEFAULT);
	EXPECT_EQ(pp->metaDataCount, 0);
	EXPECT_EQ(pp->metaData, (::S3NameValue*)0);
}

TEST_F(PropertiesTest, Explicit) {
	AccessPublicReadWrite access;
	Properties::Metadata metadata;
	metadata.insert(Properties::Pair("Key1", "Value1"));
	metadata.insert(Properties::Pair("Key2", "Value2"));
	metadata.insert(Properties::Pair("Key3", "Value3"));
	Properties properties("ContentType", "MD5", "CacheControl", "ContentDispositionFilename", "ContentEncoding", 12345, access, metadata);
	const ::S3PutProperties * pp = properties.getProperties();
	ASSERT_NE(pp, (::S3PutProperties*)0);
	ASSERT_NE(pp->contentType, (char *)0);
	EXPECT_EQ(std::strcmp(pp->contentType, "ContentType"), 0);
	ASSERT_NE(pp->md5, (char *)0);
	EXPECT_EQ(std::strcmp(pp->md5, "MD5"), 0);
	ASSERT_NE(pp->cacheControl, (char *)0);
	EXPECT_EQ(std::strcmp(pp->cacheControl, "CacheControl"), 0);
	ASSERT_NE(pp->contentDispositionFilename, (char *)0);
	EXPECT_EQ(std::strcmp(pp->contentDispositionFilename, "ContentDispositionFilename"), 0);
	ASSERT_NE(pp->contentEncoding, (char *)0);
	EXPECT_EQ(std::strcmp(pp->contentEncoding, "ContentEncoding"), 0);
	EXPECT_EQ(pp->expires, 12345);
	EXPECT_EQ(pp->cannedAcl, S3CannedAclPublicReadWrite);
	EXPECT_EQ(pp->metaDataCount, 3);
	ASSERT_NE(pp->metaData, (::S3NameValue*)0);
	ASSERT_NE(pp->metaData[0].name, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[0].name, "Key1"), 0);
	ASSERT_NE(pp->metaData[0].value, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[0].value, "Value1"), 0);
	ASSERT_NE(pp->metaData[1].name, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[1].name, "Key2"), 0);
	ASSERT_NE(pp->metaData[1].value, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[1].value, "Value2"), 0);
	ASSERT_NE(pp->metaData[2].name, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[2].name, "Key3"), 0);
	ASSERT_NE(pp->metaData[2].value, (char *)0);
	EXPECT_EQ(std::strcmp(pp->metaData[2].value, "Value3"), 0);
}

}
}
}
}

#endif
