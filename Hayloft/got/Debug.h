/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_DEBUG_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_DEBUG_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/hayloft/Debug.h"
#include "com/diag/grandote/stdlib.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

typedef Fixture DebugTest;

TEST_F(DebugTest, Global) {
    bool value = Debugging;
    Debugging = !value;
    EXPECT_EQ(Debugging, !value);
    Debugging = value;
    EXPECT_EQ(Debugging, value);
}

static Debug debug(false);

TEST_F(DebugTest, Static) {
    bool value = debug;
    EXPECT_EQ(debug, false);
    debug = !value;
    EXPECT_EQ(debug, true);
    debug = value;
    EXPECT_EQ(debug, false);
}

TEST_F(DebugTest, Stack) {
    Debug debug(true);
    bool value = debug;
    EXPECT_EQ(debug, true);
    debug = !value;
    EXPECT_EQ(debug, false);
    debug = value;
    EXPECT_EQ(debug, true);
}

TEST_F(DebugTest, Heap) {
    const char * environment = std::getenv(Debug::DEBUG_ENV());
    if (environment != (char *)0) {
        ::unsetenv(Debug::DEBUG_ENV());
    }
    Debug * debugp = new Debug();
    bool value = *debugp;
    EXPECT_EQ(*debugp, false);
    *debugp = !value;
    EXPECT_EQ(*debugp, true);
    *debugp = value;
    EXPECT_EQ(*debugp, false);
    delete debugp;
    if (environment != (char *)0) {
        ::setenv(Debug::DEBUG_ENV(), environment, !0);
    }
}

TEST_F(DebugTest, Environment) {
    const char * environment = std::getenv(Debug::DEBUG_ENV());
    ::setenv(Debug::DEBUG_ENV(), "", !0);
    Debug * debugp = new Debug();
    bool value = *debugp;
    EXPECT_EQ(*debugp, true);
    *debugp = !value;
    EXPECT_EQ(*debugp, false);
    *debugp = value;
    EXPECT_EQ(*debugp, true);
    delete debugp;
    if (environment != (char *)0) {
        ::setenv(Debug::DEBUG_ENV(), environment, !0);
    } else {
        ::unsetenv(Debug::DEBUG_ENV());
    }
}

}
}
}

#endif
