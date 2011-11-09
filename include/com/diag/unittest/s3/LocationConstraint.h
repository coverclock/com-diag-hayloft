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
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), LocationConstraint::DEFAULT()), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(LocationConstraint::DEFAULT()));
}

TEST_F(LocationConstraintTest, EuropeanUnion) {
	static const char EXPECTED[] = "EU";
	LocationConstraintEuropeanUnion locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), EXPECTED), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(EXPECTED));
}

TEST_F(LocationConstraintTest, UnitedStatesWest1) {
	static const char EXPECTED[] = "us-west-1";
	LocationConstraintUnitedStatesWest1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), EXPECTED), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(EXPECTED));
}

TEST_F(LocationConstraintTest, AsiaPacificSouthEast1) {
	static const char EXPECTED[] = "ap-southeast-1";
	LocationConstraintAsiaPacificSouthEast1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), EXPECTED), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(EXPECTED));
}

TEST_F(LocationConstraintTest, AsiaPacificNorthEast1) {
	static const char EXPECTED[] = "ap-northeast-1";
	LocationConstraintAsiaPacificNorthEast1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), EXPECTED), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(EXPECTED));
}

TEST_F(LocationConstraintTest, UnitedStatesClassic) {
	static const char EXPECTED[] = "";
	LocationConstraintUnitedStatesClassic locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), EXPECTED), 0);
	EXPECT_EQ(locationconstraint.getLength(), std::strlen(EXPECTED));
}

static const char * locationconstraintfunction(const LocationConstraint & constraint = LocationConstraint()) {
	return constraint.getLocationConstraint();
}

TEST_F(LocationConstraintTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(locationconstraintfunction(), LocationConstraint::DEFAULT()), 0);
}

}
}
}
}

#endif
