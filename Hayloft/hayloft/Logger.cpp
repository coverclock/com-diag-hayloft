/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/CriticalSection.h"
#include "com/diag/grandote/LogOutput.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/uint16_Number.h"

namespace com {
namespace diag {
namespace hayloft {

static Mutex mutex;

static Mutex serializer;

static Logger * instant = 0;

Logger * Logger::singleton = 0;

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

Logger & Logger::instance(Logger & that) {
	CriticalSection guard(mutex);
	singleton = &that;
	return *singleton;
}

Logger & Logger::instance() {
	CriticalSection guard(mutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}

Logger & Logger::setMask() {
	const char * name = MASK_ENV();
	const char * string = std::getenv(name);
	if (string != 0) {
		Mask value;
		size_t length;
		if (::com::diag::grandote::uint16_Number(string, value, length)) {
			mask = value;
		}
	}
	return *this;
}

ssize_t Logger::emit(const char* buffer, size_t size) {
	CriticalSection guard(serializer);
	ssize_t rc = ::com::diag::grandote::Logger::emit(buffer, size);
	(output())();
	return rc;
}

void Logger::show(int level, ::com::diag::grandote::Output * display, int indent) const {
	::com::diag::grandote::Platform& pl = ::com::diag::grandote::Platform::instance();
	::com::diag::grandote::Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    ::com::diag::grandote::Logger::show(level, display, indent + 1);
    printf("%s mask=0x%x\n", sp, mask);
}

}
}
}
