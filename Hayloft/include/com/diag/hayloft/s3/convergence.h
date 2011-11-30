/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_RECOVER_
#define _H_COM_DIAG_HAYLOFT_S3_RECOVER_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Action;

extern bool complete_generic(Action & action, bool invert = false, int tries = 20, Milliseconds delay = 1000, Logger::Level level = Logger::DEBUG);

inline bool complete_until_success(Action & action, Logger::Level level = Logger::DEBUG) {
	return complete_generic(action, false, 20, 1000, level);
}

inline bool complete_until_nonexistent(Action & action, Logger::Level level = Logger::DEBUG) {
	return complete_generic(action, true, 20, 1000, level);
}

extern bool service_generic(Action & action, bool invert = false, int tries = 20, Milliseconds delay = 1000, int iterations = 100, Milliseconds timeout = 100, Logger::Level level = Logger::DEBUG);

inline bool service_until_success(Action & action, Logger::Level level = Logger::DEBUG) {
	return service_generic(action, false, 20, 1000, 100, 100, level);
}

inline bool service_until_nonexistent(Action & action, Logger::Level level = Logger::DEBUG) {
	return service_generic(action, true, 20, 1000, 100, 100, level);
}

}
}
}
}

#endif
