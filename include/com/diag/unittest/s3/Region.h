/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_LOCATIONCONSTRAINT
#define _H_COM_DIAG_UNITTEST_S3_LOCATIONCONSTRAINT

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
#include "com/diag/hayloft/s3/Region.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture RegionTest;

TEST_F(RegionTest, Heap) {
	Region * pointer = new Region;
	EXPECT_NE(pointer, (Region*)0);
	delete pointer;
}

TEST_F(RegionTest, Stack) {
	Region locationconstraint;
}

TEST_F(RegionTest, Default) {
	Region locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::DEFAULT()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::DEFAULT()));
}

TEST_F(RegionTest, Ireland) {
	RegionIreland locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::EUROPEAN_UNION_WEST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::EUROPEAN_UNION_WEST_1()));
}

TEST_F(RegionTest, NorthernCalifornia) {
	RegionNorthernCalifornia locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::UNITED_STATES_WEST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::UNITED_STATES_WEST_1()));
}

TEST_F(RegionTest, Singapore) {
	RegionSingapore locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::ASIA_PACIFIC_SOUTHEAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::ASIA_PACIFIC_SOUTHEAST_1()));
}

TEST_F(RegionTest, Tokyo) {
	RegionTokyo locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::ASIA_PACIFIC_NORTHEAST_1()));
}

TEST_F(RegionTest, UnitedStates) {
	RegionUnitedStates locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::UNITED_STATES_CLASSIC()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::UNITED_STATES_CLASSIC()));
}

TEST_F(RegionTest, NorthernVirginia) {
	RegionNorthernVirginia locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::UNITED_STATES_EAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::UNITED_STATES_EAST_1()));
}

TEST_F(RegionTest, Oregon) {
	RegionOregon locationconstraint;
	ASSERT_NE(locationconstraint.getRegion(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getRegion(), Region::UNITED_STATES_WEST_2()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(Region::UNITED_STATES_WEST_2()));
}

static const char * locationconstraintfunction(const Region & constraint = Region()) {
	return constraint.getRegion();
}

TEST_F(RegionTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(locationconstraintfunction(), Region::DEFAULT()), 0);
}

}
}
}
}

#endif
