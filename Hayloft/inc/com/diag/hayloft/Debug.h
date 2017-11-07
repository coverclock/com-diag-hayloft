/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_DEBUG_
#define _H_COM_DIAG_HAYLOFT_DEBUG_

/**
 * @file
 *
 * Copyright 2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/grandote/stdlib.h"

namespace com {
namespace diag {
namespace hayloft {

class Debug {

protected:

    bool debug;

public:

    static const bool DEFAULT = false;

    static const char * DEBUG_ENV() { return "COM_DIAG_HAYLOFT_DEBUG"; }

    explicit Debug(bool value) { debug = value; }

    explicit Debug() {
        const char * value = std::getenv(DEBUG_ENV());
        debug = (value == (const char *)0) ? DEFAULT : true;
    }

    operator bool() const { return debug; }

    bool operator=(bool value) { debug = value; return debug; }

};

extern Debug Debugging;

}
}
}

#endif
