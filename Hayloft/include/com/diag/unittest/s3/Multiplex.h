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
#include "com/diag/hayloft/s3/Simplex.h"

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
	EXPECT_NE(multiplex->getPending(), (Pending*)0);
	delete multiplex;
}

TEST_F(MultiplexTest, Stack) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_NE(multiplex.getPending(), (Pending*)0);
}

TEST_F(MultiplexTest, Complete) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_EQ(multiplex.complete(), 0);
}

TEST_F(MultiplexTest, Iterate) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	int actions;
	EXPECT_EQ(multiplex.iterate(actions), 0);
	EXPECT_EQ(actions, 0);
}

TEST_F(MultiplexTest, Ready) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_EQ(multiplex.ready(), 0);
}

TEST_F(MultiplexTest, Plex) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_NE(multiplex.getPending(), (Pending*)0);
	Plex plex(multiplex.getPending());
	EXPECT_NE(plex.getPending(), (Pending*)0);
	EXPECT_EQ(multiplex.getPending(), plex.getPending());
}

TEST_F(MultiplexTest, Simplex) {
	Simplex simplex;
	EXPECT_EQ(simplex.getPending(), (Pending*)0);
}

}
}
}
}

#endif
