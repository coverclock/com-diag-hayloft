/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_MULTIPLEX_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_MULTIPLEX_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "Environment.h"
#include "Fixture.h"
#include "com/diag/hayloft/Multiplex.h"
#include "com/diag/hayloft/Simplex.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture MultiplexTest;

TEST_F(MultiplexTest, Heap) {
	Multiplex * multiplex = new Multiplex;
	EXPECT_NE(multiplex, (Multiplex*)0);
	EXPECT_TRUE((*multiplex) == true);
	EXPECT_NE(multiplex->getHandle(), (Handle*)0);
	delete multiplex;
}

TEST_F(MultiplexTest, Stack) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_NE(multiplex.getHandle(), (Handle*)0);
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
	EXPECT_EQ(multiplex.ready(), Multiplex::PENDING);
}

TEST_F(MultiplexTest, Plex) {
	Multiplex multiplex;
	EXPECT_TRUE(multiplex == true);
	EXPECT_NE(multiplex.getHandle(), (Handle*)0);
	Plex plex(multiplex.getHandle());
	EXPECT_NE(plex.getHandle(), (Handle*)0);
	EXPECT_EQ(multiplex.getHandle(), plex.getHandle());
}

TEST_F(MultiplexTest, Simplex) {
	Simplex simplex;
	EXPECT_EQ(simplex.getHandle(), (Handle*)0);
}

}
}
}

#endif
