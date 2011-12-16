/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_SELECTION_
#define _H__COM_DIAG_UNITTEST_S3_SELECTION_

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
#include "com/diag/hayloft/s3/Selection.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture SelectionTest;

TEST_F(SelectionTest, Heap) {
	Selection * selection = new Selection;
	ASSERT_NE(selection->getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(selection->getPrefix(), ""), 0);
	ASSERT_NE(selection->getMarker(), (char *)0);
	EXPECT_EQ(std::strcmp(selection->getMarker(), ""), 0);
	ASSERT_NE(selection->getDelimiter(), (char *)0);
	EXPECT_EQ(std::strcmp(selection->getDelimiter(), ""), 0);
	EXPECT_EQ(selection->getMaximum(), intmaxof(int));
	delete selection;
}

TEST_F(SelectionTest, Stack) {
	Selection selection;
	ASSERT_NE(selection.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getPrefix(), ""), 0);
	ASSERT_NE(selection.getMarker(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getMarker(), ""), 0);
	ASSERT_NE(selection.getDelimiter(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getDelimiter(), ""), 0);
	EXPECT_EQ(selection.getMaximum(), intmaxof(int));
}

TEST_F(SelectionTest, Explicit) {
	Selection selection("Prefix", "Marker", "Delimiter", 1234);
	ASSERT_NE(selection.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getPrefix(), "Prefix"), 0);
	ASSERT_NE(selection.getMarker(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getMarker(), "Marker"), 0);
	ASSERT_NE(selection.getDelimiter(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getDelimiter(), "Delimiter"), 0);
	EXPECT_EQ(selection.getMaximum(), 1234);
}

TEST_F(SelectionTest, Settors) {
	Selection selection;
	selection.setPrefix().setPrefix("Prefix");
	ASSERT_NE(selection.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getPrefix(), "Prefix"), 0);
	selection.setMarker().setMarker("Marker");
	ASSERT_NE(selection.getMarker(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getMarker(), "Marker"), 0);
	selection.setDelimiter().setDelimiter("Delimiter");
	ASSERT_NE(selection.getDelimiter(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getDelimiter(), "Delimiter"), 0);
	selection.setMaximum().setMaximum(1234);
	EXPECT_EQ(selection.getMaximum(), 1234);
	selection.setPrefix().setMarker().setDelimiter().setMaximum();
	ASSERT_NE(selection.getPrefix(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getPrefix(), ""), 0);
	ASSERT_NE(selection.getMarker(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getMarker(), ""), 0);
	ASSERT_NE(selection.getDelimiter(), (char *)0);
	EXPECT_EQ(std::strcmp(selection.getDelimiter(), ""), 0);
	EXPECT_EQ(selection.getMaximum(), intmaxof(int));
}
}
}
}
}

#endif
