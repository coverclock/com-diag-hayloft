/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_SECONDS_
#define _H_COM_DIAG_UNITTEST_SECONDS_

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
#include "com/diag/hayloft/Seconds.h"
#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture SecondsTest;

TEST_F(SecondsTest, Seconds) {
	Seconds seconds;
	// This is one second past the UNIX Epoch.
	Epochalseconds then = seconds.then(1970, 1, 1, 0, 0, 1);
	EXPECT_EQ(then, 1);
	Epochalseconds now = seconds.now();
	printf("NOW=%lld\n", now);
	const char * zulu = seconds.zulu(now);
	ASSERT_NE(zulu, (char *)0);
	printf("ZULU=\"%s\"\n", zulu);
	const char * juliet = seconds.juliet(now);
	ASSERT_NE(juliet, (char *)0);
	printf("JULIET=\"%s\"\n", juliet);
	ASSERT_EQ(std::system("date"), 0);
}

}
}
}

#endif
