/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/stdlib.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {

const char * set(const char * arg, const char * nam, const char * str) {
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
