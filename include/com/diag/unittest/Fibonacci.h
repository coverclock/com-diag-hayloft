/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_FIBONACCI_
#define _H__COM_DIAG_UNITTEST_S3_FIBONACCI_

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
#include "com/diag/hayloft/Fibonacci.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture FibonacciTest;

TEST_F(FibonacciTest, Sanity) {
	Fibonacci fibonacci;
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.next(), 1);
	EXPECT_EQ(fibonacci.next(), 2);
	EXPECT_EQ(fibonacci.next(), 3);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ(fibonacci.next(), 8);
	fibonacci.reset();
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.next(), 1);
	EXPECT_EQ(fibonacci.next(), 2);
	EXPECT_EQ(fibonacci.next(), 3);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ(fibonacci.next(), 8);
	fibonacci.reset();
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ((unsigned int)fibonacci, 1);
	EXPECT_EQ((unsigned int)fibonacci, 2);
	EXPECT_EQ((unsigned int)fibonacci, 3);
	EXPECT_EQ((unsigned int)fibonacci, 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ((unsigned int)fibonacci, 8);
}

TEST_F(FibonacciTest, Overflow) {
	Fibonacci fibonacci;
	unsigned long long sum;
	unsigned long long cum = 0;
	unsigned int was = 0;
	unsigned int now = fibonacci;
	unsigned int ndx = 0;
	printf("%10s %10s %10s %10s %10s\n", "NDX", "WAS", "NOW", "SUM", "CUM");
	while (now > was) {
		ASSERT_NE(now, 0);
		sum = was;
		sum += now;
		cum += now;
		printf("%10u %10u %10u %10llu %10llu\n", ndx, was, now, sum, cum);
		was = now;
		now = fibonacci;
		++ndx;
	}
	ASSERT_NE(now, 0);
}

}
}
}

#endif
