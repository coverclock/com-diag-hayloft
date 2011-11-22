/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_FIXTURE_
#define _H_COM_DIAG_UNITTEST_S3_FIXTURE_

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

	::com::diag::desperado::Platform & platform;
	::com::diag::desperado::FileOutput errput;
	::com::diag::desperado::LogOutput logput;
	::com::diag::desperado::Print printf;
	::com::diag::desperado::Dump dump;
	Logger & logger;

	explicit Terse()
	: platform(::com::diag::desperado::Platform::instance())
	, errput(::stderr)
	, logput(errput)
	, printf(errput)
	, dump(errput)
	, logger(Logger::instance())
	{}

protected:

	::com::diag::desperado::Output * output;
	Logger::Mask mask;

	virtual void SetUp() {
		output = &(logger.getOutput());
		mask = logger.getMask();
		logger
			.setOutput(logput)
	    	.disable(Logger::FINEST)
	    	.disable(Logger::FINER)
	    	.disable(Logger::FINE)
	    	.disable(Logger::TRACE)
	    	.disable(Logger::DEBUG)
	    	.disable(Logger::INFORMATION)
	    	.disable(Logger::CONFIGURATION)
	    	.disable(Logger::NOTICE)
	    	.enable(Logger::WARNING)
	    	.enable(Logger::ERROR)
	    	.enable(Logger::SEVERE)
	    	.enable(Logger::CRITICAL)
	    	.enable(Logger::ALERT)
	    	.enable(Logger::FATAL)
	    	.enable(Logger::EMERGENCY)
	    	.enable(Logger::PRINT)
			.setMask();
	}

	virtual void TearDown() {
		logger.setMask(mask);
		logger.setOutput(*output);
	}

};

class Verbose : public Terse {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		mask = logger.getMask();
		logger
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
	    	.enable(Logger::PRINT)
			.setMask();
	}

	virtual void TearDown() {
		logger.setMask(mask);
		logger.setOutput(*output);
	}

};

/**
 * This type is the default logging strategy. Choose Verbose or Terse. You
 * can always choose the other on specific test cases, or even create your
 * own by deriving your own class.
*/
typedef Terse Fixture;

}
}
}

#endif
