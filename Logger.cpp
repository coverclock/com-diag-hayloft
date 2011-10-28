/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdlib>
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/uint16_Number.h"

namespace com {
namespace diag {
namespace hayloft {

Logger & Logger::factory() {
    return (*(new Logger))
    	.disable(FINEST)
    	.disable(FINER)
    	.disable(FINE)
    	.disable(TRACE)
    	.disable(DEBUG)
    	.disable(INFORMATION)
    	.disable(CONFIGURATION)
    	.enable(NOTICE)
    	.enable(WARNING)
    	.enable(ERROR)
    	.enable(SEVERE)
    	.enable(CRITICAL)
    	.enable(ALERT)
    	.enable(FATAL)
    	.enable(EMERGENCY)
    	.enable(PRINT);
}

static Logger & instant = Logger::factory();

Logger * Logger::singleton = &instant;

Logger & Logger::setMask() {
	const char * name = MASK_ENV_NAME();
	const char * string = std::getenv(name);
	if (string != 0) {
		Mask value;
		size_t length;
		if (uint16_Number(string, value, length)) {
			mask = value;
		}
	}
	return *this;
}

}
}
}
