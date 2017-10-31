/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_MANIFEST_
#define _H__COM_DIAG_UNITTEST_S3_MANIFEST_

/**
 * @file
 *
 * Copyright 2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/Seconds.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/stdio.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ManifestTest;

static bool manifesttestenable = true;

static const int MANIFESTTESTLIMIT = 2500;

static const char MANIFESTTESTBUCKET[] = "ManifestTest";

struct ManifestTestInput : public ::com::diag::grandote::Input {
	virtual bool retryable(Status final, bool nonexistence = true) {
		return false;
	}
};

TEST_F(ManifestTest, Part0Disable) {
	manifesttestenable = false;
	logger.notice("ManifestTest is disabled; to run, execute subsequent sub-tests individually.\n");
}

TEST_F(ManifestTest, Part1Setup) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	BucketCreate bucketcreate(bucket, complex);
	ASSERT_TRUE(complex.start(bucketcreate));
	ASSERT_TRUE(bucketcreate.wait());
	ASSERT_TRUE(bucketcreate == true);
	ASSERT_TRUE(bucketcreate.isSuccessful());
	for (unsigned int index = 0; index < MANIFESTTESTLIMIT; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		Object object(name, bucket);
		ManifestTestInput input;
		logger.notice("creating %s\n", name);
		ObjectPut objectput(object, complex, input, 0);
		ASSERT_TRUE(complex.start(objectput));
		ASSERT_TRUE(objectput.wait());
		ASSERT_TRUE(objectput == true);
		ASSERT_TRUE(objectput.isSuccessful());
	}
}

TEST_F(ManifestTest, Part21Manifest) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	BucketManifest bucketmanifest(bucket, complex);
	ASSERT_TRUE(complex.start(bucketmanifest));
	ASSERT_TRUE(bucketmanifest.wait());
	ASSERT_TRUE(bucketmanifest == true);
	ASSERT_TRUE(bucketmanifest.isSuccessful());
	const BucketManifest::Manifest manifest = bucketmanifest.getManifest();
	ASSERT_EQ(manifest.size(), MANIFESTTESTLIMIT);
	Seconds seconds;
	for (unsigned int index = 0; index < MANIFESTTESTLIMIT; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		const BucketManifest::Entry * entry = bucketmanifest.find(name);
		ASSERT_NE(entry, (BucketManifest::Entry*)0);
		const char * key = entry->getKey();
		ASSERT_NE(key, (char *)0);
		ASSERT_EQ(std::strcmp(entry->getKey(), name), 0);
		Epochalseconds modified = entry->getModified();
		const char * timestamp = seconds.zulu(modified);
		ASSERT_NE(timestamp, (char *)0);
		const char * etag = entry->getETag();
		ASSERT_NE(etag, (char *)0);
		Octets size = entry->getSize();
		ASSERT_EQ(size, 0);
		const char * id = entry->getOwnerId();
		ASSERT_NE(id, (char *)0);
		const char * display = entry->getOwnerDisplayName();
		ASSERT_NE(display, (char *)0);
		printf("%s %s %llu %s\n", key, timestamp, size, display);
	}
}

TEST_F(ManifestTest, Part22Prefix) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection("OBJECT1");
	BucketManifest bucketmanifest(bucket, complex, selection);
	ASSERT_TRUE(complex.start(bucketmanifest));
	ASSERT_TRUE(bucketmanifest.wait());
	ASSERT_TRUE(bucketmanifest == true);
	ASSERT_TRUE(bucketmanifest.isSuccessful());
	const BucketManifest::Manifest manifest = bucketmanifest.getManifest();
	ASSERT_EQ(manifest.size(), 1000);
	Seconds seconds;
	for (unsigned int index = 1000; index < 2000; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		const BucketManifest::Entry * entry = bucketmanifest.find(name);
		ASSERT_NE(entry, (BucketManifest::Entry*)0);
		const char * key = entry->getKey();
		ASSERT_NE(key, (char *)0);
		ASSERT_EQ(std::strcmp(entry->getKey(), name), 0);
		Epochalseconds modified = entry->getModified();
		const char * timestamp = seconds.zulu(modified);
		ASSERT_NE(timestamp, (char *)0);
		const char * etag = entry->getETag();
		ASSERT_NE(etag, (char *)0);
		Octets size = entry->getSize();
		ASSERT_EQ(size, 0);
		const char * id = entry->getOwnerId();
		ASSERT_NE(id, (char *)0);
		const char * display = entry->getOwnerDisplayName();
		ASSERT_NE(display, (char *)0);
		printf("%s %s %llu %s\n", key, timestamp, size, display);
	}
}

TEST_F(ManifestTest, Part23Next) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection(0, "OBJECT0499.txt", 0, 1500);
	BucketManifest bucketmanifest(bucket, complex, selection);
	ASSERT_TRUE(complex.start(bucketmanifest));
	ASSERT_TRUE(bucketmanifest.wait());
	ASSERT_TRUE(bucketmanifest == true);
	ASSERT_TRUE(bucketmanifest.isSuccessful());
	const BucketManifest::Manifest manifest = bucketmanifest.getManifest();
	ASSERT_EQ(manifest.size(), 1500);
	Seconds seconds;
	for (unsigned int index = 500; index < 2000; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		const BucketManifest::Entry * entry = bucketmanifest.find(name);
		ASSERT_NE(entry, (BucketManifest::Entry*)0);
		const char * key = entry->getKey();
		ASSERT_NE(key, (char *)0);
		ASSERT_EQ(std::strcmp(entry->getKey(), name), 0);
		Epochalseconds modified = entry->getModified();
		const char * timestamp = seconds.zulu(modified);
		ASSERT_NE(timestamp, (char *)0);
		const char * etag = entry->getETag();
		ASSERT_NE(etag, (char *)0);
		Octets size = entry->getSize();
		ASSERT_EQ(size, 0);
		const char * id = entry->getOwnerId();
		ASSERT_NE(id, (char *)0);
		const char * display = entry->getOwnerDisplayName();
		ASSERT_NE(display, (char *)0);
		printf("%s %s %llu %s\n", key, timestamp, size, display);
	}
}

TEST_F(ManifestTest, Part24Common) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection("OBJECT1", 0, ".txt");
	BucketManifest bucketmanifest(bucket, complex, selection);
	ASSERT_TRUE(complex.start(bucketmanifest));
	ASSERT_TRUE(bucketmanifest.wait());
	ASSERT_TRUE(bucketmanifest == true);
	ASSERT_TRUE(bucketmanifest.isSuccessful());
	const BucketManifest::Common common = bucketmanifest.getCommon();
	ASSERT_EQ(common.size(), 1000);
	unsigned int index = 1000;
	BucketManifest::Common::const_iterator here = common.begin();
	BucketManifest::Common::const_iterator there = common.end();
	while (here != there) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		ASSERT_EQ(*here, std::string(name));
		printf("%s\n", (*here).c_str());
		++index;
		++here;
	}
}

TEST_F(ManifestTest, Part9Cleanup) {
	if (!manifesttestenable) { return; }
	Complex complex;
	Bucket bucket(MANIFESTTESTBUCKET);
	for (unsigned int index = 0; index < MANIFESTTESTLIMIT; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		Object object(name, bucket);
		ObjectDelete objectdelete(object, complex);
		logger.notice("deleting %s\n", name);
		ASSERT_TRUE(complex.start(objectdelete));
		ASSERT_TRUE(objectdelete.wait());
		ASSERT_TRUE(objectdelete == true);
		ASSERT_TRUE(objectdelete.isSuccessful());
	}
	BucketDelete bucketdelete(bucket, complex);
	ASSERT_TRUE(complex.start(bucketdelete));
	ASSERT_TRUE(bucketdelete.wait());
	ASSERT_TRUE(bucketdelete == true);
	ASSERT_TRUE(bucketdelete.isSuccessful());
}

}
}
}
}

#endif
