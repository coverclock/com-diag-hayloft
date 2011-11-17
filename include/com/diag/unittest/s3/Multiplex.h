/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_MULTIPLEX_
#define _H__COM_DIAG_UNITTEST_S3_MULTIPLEX_

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
#include "com/diag/hayloft/s3/Multiplex.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture MultiplexTest;

TEST_F(MultiplexTest, Heap) {
	Multiplex * multiplex = new Multiplex;
	EXPECT_NE(multiplex, (Multiplex*)0);
	EXPECT_TRUE((*multiplex) == true);
	EXPECT_NE(multiplex->getRequests(), (S3RequestContext *)0);
	delete multiplex;
}

TEST_F(MultiplexTest, Stack) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_NE(multiplex.getRequests(), (S3RequestContext *)0);
}

TEST_F(MultiplexTest, All) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_TRUE(multiplex.all());
}

TEST_F(MultiplexTest, Once) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	int pending;
	EXPECT_TRUE(multiplex.once(pending));
	EXPECT_EQ(pending, 0);
}

TEST_F(MultiplexTest, Ready) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_EQ(multiplex.ready(), 0);
}

}
}
}
}

#endif
