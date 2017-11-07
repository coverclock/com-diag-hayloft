/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNITTEST_FIXTURE_
#define _H_COM_DIAG_HAYLOFT_UNITTEST_FIXTURE_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "gtest/gtest.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/grandote/stdio.h"
#include "com/diag/grandote/FileOutput.h"
#include "com/diag/grandote/LogOutput.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Dump.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/hayloft/Debug.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;
using namespace ::com::diag::hayloft;

class Test : public ::testing::Test {

public:

    static inline int limit(int def) {
        static const char LIMIT[] = "COM_DIAG_HAYLOFT_UNITTEST_LIMIT";
        const char * val = std::getenv(LIMIT);
        return (val != (char *)0) ? std::atoi(val) : def;
    }

    static inline Milliseconds timeout(Milliseconds def) {
        static const char LIMIT[] = "COM_DIAG_HAYLOFT_UNITTEST_TIMEOUT";
        const char * val = std::getenv(LIMIT);
        return (val != (char *)0) ? std::atoll(val) : def;
    }

protected:

	FileOutput errput;
	LogOutput logput;
	Output * output;
	MaskableLogger::Mask mask;

public:

	MaskableLogger & logger;
	Platform & platform;
	Print printf;
	Dump dump;

	explicit Test()
	: errput(::stderr)
	, logput(errput)
	, output(0)
	, mask(0)
	, logger(MaskableLogger::instance())
	, platform(Platform::instance())
	, printf(errput)
	, dump(errput)
	{}

	virtual MaskableLogger::Mask terse() {
		MaskableLogger::Mask save = logger.getMask();
		logger
			.disable(MaskableLogger::FINEST)
			.disable(MaskableLogger::FINER)
			.disable(MaskableLogger::FINE)
			.disable(MaskableLogger::TRACE)
			.disable(MaskableLogger::DEBUG)
			.disable(MaskableLogger::INFORMATION)
			.disable(MaskableLogger::CONFIGURATION)
			.enable(MaskableLogger::NOTICE)
			.enable(MaskableLogger::WARNING)
			.enable(MaskableLogger::ERROR)
			.enable(MaskableLogger::SEVERE)
			.enable(MaskableLogger::CRITICAL)
			.enable(MaskableLogger::ALERT)
			.enable(MaskableLogger::FATAL)
			.enable(MaskableLogger::EMERGENCY)
			.enable(MaskableLogger::PRINT);
		return save;
	}

	virtual MaskableLogger::Mask verbose() {
		MaskableLogger::Mask save = logger.getMask();
		logger
	    	.enable(MaskableLogger::FINEST)
	    	.enable(MaskableLogger::FINER)
	    	.enable(MaskableLogger::FINE)
	    	.enable(MaskableLogger::TRACE)
	    	.enable(MaskableLogger::DEBUG)
	    	.enable(MaskableLogger::INFORMATION)
	    	.enable(MaskableLogger::CONFIGURATION)
	    	.enable(MaskableLogger::NOTICE)
	    	.enable(MaskableLogger::WARNING)
	    	.enable(MaskableLogger::ERROR)
	    	.enable(MaskableLogger::SEVERE)
	    	.enable(MaskableLogger::CRITICAL)
	    	.enable(MaskableLogger::ALERT)
	    	.enable(MaskableLogger::FATAL)
	    	.enable(MaskableLogger::EMERGENCY)
	    	.enable(MaskableLogger::PRINT);
		return save;
	}

	virtual void restore(MaskableLogger::Mask save) {
		logger.setMask(save);
	}

};

class Fixture : public Test {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		logger.setOutput(logput);
		mask = Debugging ? verbose() : terse();
		logger.setMask();
	}

	virtual void TearDown() {
		restore(mask);
		logger.setOutput(*output);
	}

};

}
}
}

#endif
