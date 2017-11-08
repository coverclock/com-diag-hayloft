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

/**
 * Debug is a utility class that is used at run-time initialization to extract
 * the value of the variable COM_DIAG_HAYLOFT_DEBUG from the process environment and,
 * if it exists at all, regardless of its value, enable debug mode in Hayloft. Several
 * Hayloft classes alter their behavior of debug mode is enabled.
 */
class Debug {

protected:

    bool debug;

public:

    /**
     * The default debug mode is for it to be disabled.
     */
    static const bool DEFAULT = false;

    /**
     * Return the string that identifies the environmental variable that
     * if defined, regardless of value, enables debug mode.
     * @return the environmmental variable name.
     */
    static const char * DEBUG_ENV() { return "COM_DIAG_HAYLOFT_DEBUG"; }

    /**
     * Construct an object that can be used to indicate the desired debug mode.
     * @param value is the desired debug mode state.
     */
    explicit Debug(bool value) { debug = value; }

    /**
     * Construct an object that indicates the debug mode state based on the
     * the presence of the environmental variable.
     */
    explicit Debug() {
        const char * value = std::getenv(DEBUG_ENV());
        debug = (value == (const char *)0) ? DEFAULT : true;
    }

    /**
     * Return the current debug mode state.
     * @return the current debug mode state.
     */
    operator bool() const { return debug; }

    /**
     * Set and return the desired debug mode state.
     * @param value is the desired debug mode state.
     * @return the current debug mode state.
     */
    bool operator=(bool value) { debug = value; return debug; }

};

/**
 * This global variable is initialized at run-time to indicate whether Hayloft debug
 * mode is enabled or disabled.
 */
extern Debug Debugging;

}
}
}

#endif
