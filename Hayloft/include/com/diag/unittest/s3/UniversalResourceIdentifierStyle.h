/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_UNIVERSALRESOURCEIDENTIFIERSTYLE
#define _H_COM_DIAG_UNITTEST_S3_UNIVERSALRESOURCEIDENTIFIERSTYLE

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
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture UniversalResourceIdentifierStyleTest;

TEST_F(UniversalResourceIdentifierStyleTest, Heap) {
	UniversalResourceIdentifierStyle * pointer = new UniversalResourceIdentifierStyle;
	EXPECT_NE(pointer, (UniversalResourceIdentifierStyle*)0);
	delete pointer;
}

TEST_F(UniversalResourceIdentifierStyleTest, Stack) {
	UniversalResourceIdentifierStyle style;
}

TEST_F(UniversalResourceIdentifierStyleTest, Default) {
	UniversalResourceIdentifierStyle style;
	EXPECT_EQ(style.getStyle(), UniversalResourceIdentifierStyle::DEFAULT);
}

TEST_F(UniversalResourceIdentifierStyleTest, VirtualHost) {
	UniversalResourceIdentifierStyleVirtualHost style;
	EXPECT_EQ(style.getStyle(), ::S3UriStyleVirtualHost);
}

TEST_F(UniversalResourceIdentifierStyleTest, Path) {
	UniversalResourceIdentifierStylePath style;
	EXPECT_EQ(style.getStyle(), ::S3UriStylePath);
}

static int universalresourceidentifierstylefunction(const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle()) {
	return style.getStyle();
}

TEST_F(UniversalResourceIdentifierStyleTest, DefaultFunctionArgument) {
	EXPECT_EQ(universalresourceidentifierstylefunction(), UniversalResourceIdentifierStyle::DEFAULT);
}

}
}
}
}

#endif
