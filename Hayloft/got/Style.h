/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_STYLE_
#define _H_COM_DIAG_UNITTEST_S3_STYLE_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/Style.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft;

typedef Fixture StyleTest;

TEST_F(StyleTest, Heap) {
	Style * pointer = new Style;
	EXPECT_NE(pointer, (Style*)0);
	delete pointer;
}

TEST_F(StyleTest, Stack) {
	Style style;
}

TEST_F(StyleTest, Default) {
	Style style;
	EXPECT_EQ(style.getStyle(), Style::DEFAULT);
}

TEST_F(StyleTest, VirtualHost) {
	StyleVirtualHost style;
	EXPECT_EQ(style.getStyle(), ::S3UriStyleVirtualHost);
}

TEST_F(StyleTest, Path) {
	StylePath style;
	EXPECT_EQ(style.getStyle(), ::S3UriStylePath);
}

static int universalresourceidentifierstylefunction(const Style & style = Style()) {
	return style.getStyle();
}

TEST_F(StyleTest, DefaultFunctionArgument) {
	EXPECT_EQ(universalresourceidentifierstylefunction(), Style::DEFAULT);
}

TEST_F(StyleTest, Settors) {
	Style style;
	style.setStyle().setStyle(::S3UriStylePath);
	EXPECT_EQ(style.getStyle(), ::S3UriStylePath);
	EXPECT_EQ(style.setStyle().getStyle(), Style::DEFAULT);
}

}
}
}
}

#endif
