/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_MANIFEST_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_MANIFEST_

/**
 * @file
 *
 * Copyright 2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/hayloft/BucketManifest.h"
#include "com/diag/hayloft/Complex.h"
#include "com/diag/grandote/Seconds.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/stdio.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef ::com::diag::hayloft::Object HayloftObject; // Resolve Object ambiguity.

typedef Fixture ManifestTest;

static bool manifesttestenable = true;

static const int MANIFESTTESTLIMIT = 2500;

static const char MANIFESTTESTBUCKET[] = "ManifestTest";

struct ManifestTestInput : public Input {
	virtual bool retryable(Status ultimate, bool nonexistence = true) {
		return false;
	}
};

TEST_F(ManifestTest, Part0Disable) {
	manifesttestenable = false;
	logger.notice("ManifestTest is disabled; to run, execute subsequent sub-tests individually.\n");
}

TEST_F(ManifestTest, Part1Setup) {
	if (!manifesttestenable) { return; }
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	BucketCreate bucketcreate(bucket, komplex);
	ASSERT_TRUE(komplex.start(bucketcreate));
	ASSERT_TRUE(bucketcreate.wait());
	ASSERT_TRUE(bucketcreate == true);
	ASSERT_TRUE(bucketcreate.isSuccessful());
	for (unsigned int index = 0; index < MANIFESTTESTLIMIT; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		HayloftObject object(name, bucket);
		ManifestTestInput input;
		logger.notice("creating %s\n", name);
		ObjectPut objectput(object, komplex, input, 0);
		ASSERT_TRUE(komplex.start(objectput));
		ASSERT_TRUE(objectput.wait());
		ASSERT_TRUE(objectput == true);
		ASSERT_TRUE(objectput.isSuccessful());
	}
}

TEST_F(ManifestTest, Part21Manifest) {
	if (!manifesttestenable) { return; }
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	BucketManifest bucketmanifest(bucket, komplex);
	ASSERT_TRUE(komplex.start(bucketmanifest));
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
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection("OBJECT1");
	BucketManifest bucketmanifest(bucket, komplex, selection);
	ASSERT_TRUE(komplex.start(bucketmanifest));
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
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection(0, "OBJECT0499.txt", 0, 1500);
	BucketManifest bucketmanifest(bucket, komplex, selection);
	ASSERT_TRUE(komplex.start(bucketmanifest));
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
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	Selection selection("OBJECT1", 0, ".txt");
	BucketManifest bucketmanifest(bucket, komplex, selection);
	ASSERT_TRUE(komplex.start(bucketmanifest));
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
	Complex komplex;
	Bucket bucket(MANIFESTTESTBUCKET);
	for (unsigned int index = 0; index < MANIFESTTESTLIMIT; ++index) {
		char name[sizeof("OBJECT????.txt")] = { 0 };
		snprintf(name, sizeof(name), "OBJECT%4.4u.txt", index);
		HayloftObject object(name, bucket);
		ObjectDelete objectdelete(object, komplex);
		logger.notice("deleting %s\n", name);
		ASSERT_TRUE(komplex.start(objectdelete));
		ASSERT_TRUE(objectdelete.wait());
		ASSERT_TRUE(objectdelete == true);
		ASSERT_TRUE(objectdelete.isSuccessful());
	}
	BucketDelete bucketdelete(bucket, komplex);
	ASSERT_TRUE(komplex.start(bucketdelete));
	ASSERT_TRUE(bucketdelete.wait());
	ASSERT_TRUE(bucketdelete == true);
	ASSERT_TRUE(bucketdelete.isSuccessful());
}

}
}
}

#endif
