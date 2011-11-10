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
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture LocationConstraintTest;

TEST_F(LocationConstraintTest, Heap) {
	LocationConstraint * pointer = new LocationConstraint;
	EXPECT_NE(pointer, (LocationConstraint*)0);
	delete pointer;
}

TEST_F(LocationConstraintTest, Stack) {
	LocationConstraint locationconstraint;
}

TEST_F(LocationConstraintTest, Default) {
	LocationConstraint locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::DEFAULT()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::DEFAULT()));
}

TEST_F(LocationConstraintTest, Ireland) {
	LocationConstraintIreland locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::EUROPEAN_UNION_WEST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::EUROPEAN_UNION_WEST_1()));
}

TEST_F(LocationConstraintTest, NorthernCalifornia) {
	LocationConstraintNorthernCalifornia locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::UNITED_STATES_WEST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::UNITED_STATES_WEST_1()));
}

TEST_F(LocationConstraintTest, Singapore) {
	LocationConstraintSingapore locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::ASIA_PACIFIC_SOUTHEAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::ASIA_PACIFIC_SOUTHEAST_1()));
}

TEST_F(LocationConstraintTest, Tokyo) {
	LocationConstraintTokyo locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::ASIA_PACIFIC_NORTHEAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::ASIA_PACIFIC_NORTHEAST_1()));
}

TEST_F(LocationConstraintTest, UnitedStates) {
	LocationConstraintUnitedStates locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::UNITED_STATES_CLASSIC()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::UNITED_STATES_CLASSIC()));
}

TEST_F(LocationConstraintTest, NorthernVirginia) {
	LocationConstraintNorthernVirginia locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::UNITED_STATES_EAST_1()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::UNITED_STATES_EAST_1()));
}

TEST_F(LocationConstraintTest, Oregon) {
	LocationConstraintOregon locationconstraint;
	ASSERT_NE(locationconstraint.getConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getConstraint(), LocationConstraint::UNITED_STATES_WEST_2()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::UNITED_STATES_WEST_2()));
}

static const char * locationconstraintfunction(const LocationConstraint & constraint = LocationConstraint()) {
	return constraint.getConstraint();
}

TEST_F(LocationConstraintTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(locationconstraintfunction(), LocationConstraint::DEFAULT()), 0);
}

}
}
}
}

#endif
