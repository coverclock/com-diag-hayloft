/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_SET_
#define _H_COM_DIAG_HAYLOFT_SET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */


namespace com {
namespace diag {
namespace hayloft {

/**
 * Gets a C string, typically to use as an argument, from a provided C string,
 * from a named environmental variable, or from a default C string. Typically
 * used in constructor initialization lists.
 * @param arg points to a provided value.
 * @param nam points to an environmental variable name.
 * @param str points to a default value.
 * @return a pointer to the resulting value or NULL if none.
 */
extern const char * set(const char * arg = 0, const char * nam = 0, const char * str = 0);

}
}
}

#endif
