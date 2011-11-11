/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_FIXTURE
#define _H_COM_DIAG_UNITTEST_S3_FIXTURE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

class Terse : public ::testing::Test {

public:

	static ::com::diag::desperado::Print printf;
	static ::com::diag::desperado::Dump dump;

	static ::com::diag::desperado::FileOutput errput;
	static ::com::diag::desperado::LogOutput logput;

protected:

	::com::diag::desperado::Output * output;
	Logger::Mask mask;

	virtual void SetUp() {
		output = &(Logger::instance().getOutput());
		mask = Logger::instance().getMask();
		Logger::instance()
			.setOutput(logput)
	    	.disable(Logger::FINEST)
	    	.disable(Logger::FINER)
	    	.disable(Logger::FINE)
	    	.disable(Logger::TRACE)
	    	.disable(Logger::DEBUG)
	    	.disable(Logger::INFORMATION)
	    	.disable(Logger::CONFIGURATION)
	    	.disable(Logger::NOTICE)
	    	.disable(Logger::WARNING)
	    	.enable(Logger::ERROR)
	    	.enable(Logger::SEVERE)
	    	.enable(Logger::CRITICAL)
	    	.enable(Logger::ALERT)
	    	.enable(Logger::FATAL)
	    	.enable(Logger::EMERGENCY)
	    	.enable(Logger::PRINT);
	}

	virtual void TearDown() {
		Logger::instance().setMask(mask);
		Logger::instance().setOutput(*output);
	}

};

::com::diag::desperado::FileOutput Terse::errput(::stderr);
::com::diag::desperado::LogOutput Terse::logput(Terse::errput);
::com::diag::desperado::Print Terse::printf(Terse::errput);
::com::diag::desperado::Dump Terse::dump(Terse::errput);

class Verbose : public Terse {

protected:

	virtual void SetUp() {
		output = &(Logger::instance().getOutput());
		mask = Logger::instance().getMask();
		Logger::instance()
			.setOutput(logput)
	    	.enable(Logger::FINEST)
	    	.enable(Logger::FINER)
	    	.enable(Logger::FINE)
	    	.enable(Logger::TRACE)
	    	.enable(Logger::DEBUG)
	    	.enable(Logger::INFORMATION)
	    	.enable(Logger::CONFIGURATION)
	    	.enable(Logger::NOTICE)
	    	.enable(Logger::WARNING)
	    	.enable(Logger::ERROR)
	    	.enable(Logger::SEVERE)
	    	.enable(Logger::CRITICAL)
	    	.enable(Logger::ALERT)
	    	.enable(Logger::FATAL)
	    	.enable(Logger::EMERGENCY)
	    	.enable(Logger::PRINT);
	}

	virtual void TearDown() {
		Logger::instance().setMask(mask);
		Logger::instance().setOutput(*output);
	}

};

typedef Verbose Fixture;

}
}
}

#endif
