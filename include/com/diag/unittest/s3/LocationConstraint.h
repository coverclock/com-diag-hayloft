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

#include <cstring>
#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture LocationConstraintTest;

TEST_F(LocationConstraintTest, Default) {
	LocationConstraint locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), ""), 0);
}

TEST_F(LocationConstraintTest, EuropeanUnion) {
	LocationConstraintEuropeanUnion locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), "EU"), 0);
}

TEST_F(LocationConstraintTest, UnitedStatesWest1) {
	LocationConstraintUnitedStatesWest1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), "us-west-1"), 0);
}

TEST_F(LocationConstraintTest, AsiaPacificSouthEast1) {
	LocationConstraintAsiaPacificSouthEast1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), "ap-southeast-1"), 0);
}

TEST_F(LocationConstraintTest, AsiaPacificNorthEast1) {
	LocationConstraintAsiaPacificNorthEast1 locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), "ap-northeast-1"), 0);
}

TEST_F(LocationConstraintTest, UnitedStatesClassic) {
	LocationConstraintUnitedStatesClassic locationconstraint;
	ASSERT_NE(locationconstraint.getLocationConstraint(), (char *)0);
	EXPECT_EQ(std::strcmp(locationconstraint.getLocationConstraint(), ""), 0);
}

}
}
}
}

#endif
