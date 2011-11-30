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

class Test : public ::testing::Test {

protected:

	::com::diag::desperado::FileOutput errput;
	::com::diag::desperado::LogOutput logput;
	::com::diag::desperado::Output * output;
	Logger::Mask mask;

public:

	Logger & logger;
	::com::diag::desperado::Platform & platform;
	::com::diag::desperado::Print printf;
	::com::diag::desperado::Dump dump;

	explicit Test()
	: errput(::stderr)
	, logput(errput)
	, output(0)
	, mask(0)
	, logger(Logger::instance())
	, platform(::com::diag::desperado::Platform::instance())
	, printf(errput)
	, dump(errput)
	{}

	virtual Logger::Mask terse() {
		Logger::Mask save = logger.getMask();
		logger
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
			.enable(Logger::PRINT);
		return save;
	}

	virtual Logger::Mask verbose() {
		Logger::Mask save = logger.getMask();
		logger
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
		return save;
	}

	virtual void restore(Logger::Mask save) {
		logger.setMask(save);
	}

};

class Terse : public Test {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		logger.setOutput(logput);
		mask = terse();
		logger.setMask();
	}

	virtual void TearDown() {
		restore(mask);
		logger.setOutput(*output);
	}

};

class Verbose : public Test {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		logger.setOutput(logput);
		mask = verbose();
		logger.setMask();
	}

	virtual void TearDown() {
		restore(mask);
		logger.setOutput(*output);
	}

};

/**
 * This type is the default logging strategy. Choose Verbose or Terse. You
 * can always choose the other on specific test cases, or even create your
 * own by deriving your own class.
*/
#if 1
typedef Terse Fixture;
#else
typedef Verbose Fixture;
#endif

}
}
}

#endif
