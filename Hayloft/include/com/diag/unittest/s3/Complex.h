/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_COMPLEX_
#define _H__COM_DIAG_UNITTEST_S3_COMPLEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Complex.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ComplexTest;

TEST_F(ComplexTest, Stack) {
	uint16_t mask = verbose();
	{
		Complex complex1;
		EXPECT_TRUE(complex1 == true);
		EXPECT_EQ(complex1.getStatus(), ::S3StatusOK);
		EXPECT_NE(complex1.getPending(), (Pending*)0);
		Complex complex2;
		EXPECT_TRUE(complex2 == true);
		EXPECT_EQ(complex2.getStatus(), ::S3StatusOK);
		EXPECT_NE(complex2.getPending(), (Pending*)0);
	}
	restore(mask);
}

}
}
}
}

#endif
