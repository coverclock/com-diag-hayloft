/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_FIBBONACCI_
#define _H__COM_DIAG_UNITTEST_S3_FIBBONACCI_

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
#include "com/diag/hayloft/Fibbonacci.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture FibbonacciTest;

TEST_F(FibbonacciTest, Sanity) {
	Fibbonacci fibbonacci;
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ(fibbonacci.next(), 1);
	EXPECT_EQ(fibbonacci.next(), 2);
	EXPECT_EQ(fibbonacci.next(), 3);
	EXPECT_EQ(fibbonacci.next(), 5);
	EXPECT_EQ(fibbonacci.get(), 5);
	EXPECT_EQ(fibbonacci.next(), 8);
	fibbonacci.reset();
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ(fibbonacci.next(), 1);
	EXPECT_EQ(fibbonacci.next(), 2);
	EXPECT_EQ(fibbonacci.next(), 3);
	EXPECT_EQ(fibbonacci.next(), 5);
	EXPECT_EQ(fibbonacci.get(), 5);
	EXPECT_EQ(fibbonacci.next(), 8);
	fibbonacci.reset();
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ(fibbonacci.get(), 1);
	EXPECT_EQ((unsigned int)fibbonacci, 1);
	EXPECT_EQ((unsigned int)fibbonacci, 2);
	EXPECT_EQ((unsigned int)fibbonacci, 3);
	EXPECT_EQ((unsigned int)fibbonacci, 5);
	EXPECT_EQ(fibbonacci.get(), 5);
	EXPECT_EQ((unsigned int)fibbonacci, 8);
}

TEST_F(FibbonacciTest, Overflow) {
	Fibbonacci fibbonacci;
	unsigned long long sum;
	unsigned long long cum = 0;
	unsigned int was = 0;
	unsigned int now = fibbonacci;
	unsigned int ndx = 0;
	printf("%10s %10s %10s %10s %10s\n", "NDX", "WAS", "NOW", "SUM", "CUM");
	while (now > was) {
		ASSERT_NE(now, 0);
		sum = was;
		sum += now;
		cum += now;
		printf("%10u %10u %10u %10llu %10llu\n", ndx, was, now, sum, cum);
		was = now;
		now = fibbonacci;
		++ndx;
	}
	ASSERT_NE(now, 0);
}

}
}
}

#endif
