/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_REGION_
#define _H_COM_DIAG_UNITTEST_S3_REGION_

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
#include "com/diag/hayloft/Region.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft;

typedef Fixture RegionTest;

TEST_F(RegionTest, Heap) {
	Region * pointer = new Region;
	EXPECT_NE(pointer, (Region*)0);
	delete pointer;
}

TEST_F(RegionTest, Stack) {
	Region region;
}

TEST_F(RegionTest, Default) {
	Region region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::DEFAULT()), 0);
}

TEST_F(RegionTest, Ireland) {
	RegionIreland region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::EUROPEAN_UNION_WEST_1()), 0);
}

TEST_F(RegionTest, NorthernCalifornia) {
	RegionNorthernCalifornia region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::UNITED_STATES_WEST_1()), 0);
}

TEST_F(RegionTest, Singapore) {
	RegionSingapore region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::ASIA_PACIFIC_SOUTHEAST_1()), 0);
}

TEST_F(RegionTest, Tokyo) {
	RegionTokyo region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
}

TEST_F(RegionTest, UnitedStates) {
	RegionUnitedStates region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::UNITED_STATES_STANDARD()), 0);
}

TEST_F(RegionTest, NorthernVirginia) {
	RegionNorthernVirginia region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::UNITED_STATES_EAST_1()), 0);
}

TEST_F(RegionTest, Oregon) {
	RegionOregon region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::UNITED_STATES_WEST_2()), 0);
}

TEST_F(RegionTest, SaoPaulo) {
	RegionSaoPaulo region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::SOUTH_AMERICA_EAST_1()), 0);
}

TEST_F(RegionTest, Environment) {
	const char * hostname = std::getenv(Region::REGION_ENV());
	static const char * REGION_VAL = "babylon-5";
	EXPECT_EQ(::setenv(Region::REGION_ENV(), REGION_VAL, !0), 0);
	Region region;
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), REGION_VAL), 0);
	if (hostname != 0) {
		EXPECT_EQ(::setenv(Region::REGION_ENV(), hostname, !0), 0);
		ASSERT_NE(std::getenv(Region::REGION_ENV()), (char *)0);
		EXPECT_EQ(std::strcmp(Region::REGION_ENV(), hostname), 0);
	} else {
		EXPECT_EQ(::unsetenv(Region::REGION_ENV()), 0);
		EXPECT_EQ(std::getenv(Region::REGION_ENV()), (char *)0);
	}
}

static const char * regionfunction(const Region & constraint = Region()) {
	return constraint.getRegion();
}

TEST_F(RegionTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(regionfunction(), Region::DEFAULT()), 0);
}

TEST_F(RegionTest, Settors) {
	Region region;
	region.setRegion().setRegion(Region::ASIA_PACIFIC_NORTHEAST_1());
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	region.setRegion();
	ASSERT_NE(region.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(region.getRegion(), Region::DEFAULT()), 0);
}

}
}
}
}

#endif
