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

#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Sets a character array pointer parameter value from a provided pointer, from
 * a named environmental variable, or from a default value. Typically used in
 * constructor initialization lists.
 * @param arg points to a provided value.
 * @param nam points to an environmental variable name.
 * @param str points to a default value.
 * @return a pointer to the resulting value or NULL if none.
 */
inline const char * set(const char * arg = 0, const char * nam = 0, const char * str = 0) {
	const char * val;
	const char * env;
	if (arg != 0) {
		val = arg;
	} else if ((nam != 0) && ((env = std::getenv(nam)) != 0)) {
		val = env;
	} else {
		val = str;
	}
	return val;
}

}
}
}

#endif
