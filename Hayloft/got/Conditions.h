/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_CONDITIONS_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_CONDITIONS_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/Conditions.h"
#include "com/diag/grandote/string.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture ConditionsTest;

TEST_F(ConditionsTest, Heap) {
	Conditions * conditions = new Conditions;
	EXPECT_EQ(conditions->getSince(), -1);
	EXPECT_EQ(conditions->getNotSince(), -1);
	ASSERT_NE(conditions->getMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions->getMatch(), ""), 0);
	ASSERT_NE(conditions->getNotMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions->getNotMatch(), ""), 0);
	delete conditions;
}

TEST_F(ConditionsTest, Stack) {
	Conditions conditions;
	EXPECT_EQ(conditions.getSince(), -1);
	EXPECT_EQ(conditions.getNotSince(), -1);
	ASSERT_NE(conditions.getMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getMatch(), ""), 0);
	ASSERT_NE(conditions.getNotMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getNotMatch(), ""), 0);
}

TEST_F(ConditionsTest, Explicit) {
	Conditions conditions(1234, 5678, "Match", "NotMatch");
	EXPECT_EQ(conditions.getSince(), 1234);
	EXPECT_EQ(conditions.getNotSince(), 5678);
	ASSERT_NE(conditions.getMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getMatch(), "Match"), 0);
	ASSERT_NE(conditions.getNotMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getNotMatch(), "NotMatch"), 0);
}

TEST_F(ConditionsTest, Settors) {
	Conditions conditions;
	conditions.setSince().setSince(1234);
	EXPECT_EQ(conditions.getSince(), 1234);
	conditions.setNotSince().setNotSince(5678);
	EXPECT_EQ(conditions.getNotSince(), 5678);
	conditions.setMatch().setMatch("Match");
	ASSERT_NE(conditions.getMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getMatch(), "Match"), 0);
	conditions.setNotMatch().setNotMatch("NotMatch");
	ASSERT_NE(conditions.getNotMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getNotMatch(), "NotMatch"), 0);
	conditions.setSince().setNotSince().setMatch().setNotMatch();
	EXPECT_EQ(conditions.getSince(), -1);
	EXPECT_EQ(conditions.getNotSince(), -1);
	ASSERT_NE(conditions.getMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getMatch(), ""), 0);
	ASSERT_NE(conditions.getNotMatch(), (char *)0);
	EXPECT_EQ(std::strcmp(conditions.getNotMatch(), ""), 0);
}

}
}
}

#endif
