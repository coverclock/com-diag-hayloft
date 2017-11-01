/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_LOGGER_
#define _H_COM_DIAG_UNITTEST_LOGGER_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/FileOutput.h"
#include "com/diag/grandote/LogOutput.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture LoggerTest;

TEST_F(LoggerTest, Defaults) {
	Logger mylogger;
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINEST));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINER));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.TRACE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.DEBUG));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.INFORMATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CONFIGURATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.NOTICE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.WARNING));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ERROR));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.SEVERE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CRITICAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ALERT));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FATAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.EMERGENCY));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, mylogger.getMask());
}

TEST_F(LoggerTest, HeapAndShow) {
	Logger * mylogger = new Logger;
	ASSERT_NE(mylogger, (Logger*)0);
	mylogger->show(0, &errput);
	delete mylogger;
}

#define LOGGER_TEST_ENABLEDISABLE_SET(_LEVEL_) \
	do { \
		if ((mask & (1 << _LEVEL_)) != 0) { \
			mylogger.enable(_LEVEL_); \
		} else { \
			mylogger.disable(_LEVEL_); \
		} \
	} while (false)

#define LOGGER_TEST_ENABLEDISABLE_TEST(_LEVEL_) \
	do { \
		if ((mask & (1 << _LEVEL_)) != 0) { \
			EXPECT_TRUE(mylogger.isEnabled(_LEVEL_)); \
		} else { \
			EXPECT_FALSE(mylogger.isEnabled(_LEVEL_)); \
		} \
	} while (false)

TEST_F(LoggerTest, EnableDisable) {
	Logger mylogger;
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINEST));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINER));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.TRACE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.DEBUG));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.INFORMATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CONFIGURATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.NOTICE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.WARNING));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ERROR));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.SEVERE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CRITICAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ALERT));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FATAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.EMERGENCY));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, mylogger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.PRINT);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, mylogger.getMask());
	}
}

TEST_F(LoggerTest, Initialization) {
	::com::diag::grandote::Output nulloutput;
	Logger mylogger;
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINEST));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINER));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.TRACE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.DEBUG));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.INFORMATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CONFIGURATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.NOTICE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.WARNING));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ERROR));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.SEVERE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CRITICAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ALERT));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FATAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.EMERGENCY));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, mylogger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.PRINT);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.PRINT);
		/**/
		mylogger.setOutput(nulloutput);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, mylogger.getMask());
	}
}

TEST_F(LoggerTest, SetMask) {
	Logger mylogger;
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINEST));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINER));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.TRACE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.DEBUG));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.INFORMATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CONFIGURATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.NOTICE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.WARNING));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ERROR));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.SEVERE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CRITICAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ALERT));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FATAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.EMERGENCY));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.PRINT));
	EXPECT_EQ((Logger::Mask)0, mylogger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		mylogger.setMask(mask);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, mylogger.getMask());
	}
}

TEST_F(LoggerTest, SetMaskEnvironment) {
	Logger mylogger;
	const char * old = std::getenv(mylogger.MASK_ENV());
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINEST));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINER));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FINE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.TRACE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.DEBUG));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.INFORMATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CONFIGURATION));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.NOTICE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.WARNING));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ERROR));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.SEVERE));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.CRITICAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.ALERT));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.FATAL));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.EMERGENCY));
	EXPECT_FALSE(mylogger.isEnabled(mylogger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, mylogger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		char string[sizeof("0xffff")];
		std::snprintf(string, sizeof(string), "0x%x", mask);
		EXPECT_EQ(::setenv(mylogger.MASK_ENV(), string, !0), 0);
		mylogger.setMask();
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(mylogger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, mylogger.getMask());
	}
	if (old == 0) {
		EXPECT_EQ(::unsetenv(mylogger.MASK_ENV()), 0);
	} else {
		EXPECT_EQ(::setenv(mylogger.MASK_ENV(), old, !0), 0);
	}
}

#define LOGGER_TEST_LOGGING_TEST(_LEVEL_, _LOGGING_) \
	do { \
		if ((mask & (1 << _LEVEL_)) != 0) { \
			EXPECT_TRUE((_LOGGING_) > 0); \
		} else { \
			EXPECT_TRUE((_LOGGING_) == 0); \
		} \
	} while (false)

TEST_F(LoggerTest, Logging) {
	::com::diag::grandote::FileOutput errput(stderr);
	::com::diag::grandote::LogOutput logput(errput);
	Logger mylogger(logput);
	for (int mask = 0; mask <= 0xffff; mask = (mask == 0) ? 1 : (mask << 1)) {
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(mylogger.PRINT);
		/**/
		LOGGER_TEST_LOGGING_TEST(mylogger.FINEST,mylogger.finest("finest\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.FINER,mylogger.finer("finer\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.FINE,mylogger.fine("fine\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.TRACE,mylogger.trace("trace\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.DEBUG,mylogger.debug("debug\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.INFORMATION,mylogger.information("information\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.CONFIGURATION,mylogger.configuration("configuration\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.NOTICE,mylogger.notice("notice\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.WARNING,mylogger.warning("warning\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.ERROR,mylogger.error("error\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.SEVERE,mylogger.severe("severe\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.CRITICAL,mylogger.critical("critical\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.ALERT,mylogger.alert("alert\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.FATAL,mylogger.fatal("fatal\n"));
		LOGGER_TEST_LOGGING_TEST(mylogger.EMERGENCY,mylogger.emergency("emergency\n"));
		/**/
		EXPECT_TRUE(mylogger.print("print\n") > 0);
	}
}

TEST_F(LoggerTest, Instance) {
	Logger & l1 = Logger::instance();
	::com::diag::grandote::FileOutput errput(stderr);
	::com::diag::grandote::LogOutput logput(errput);
	Logger mylogger(logput);
	EXPECT_NE(&l1, &mylogger);
	Logger & l2 = Logger::instance(mylogger);
	EXPECT_EQ(&mylogger, &l2);
	EXPECT_NE(&l1, &l2);
	Logger & l3 = Logger::instance();
	EXPECT_EQ(&l2, &l3);
	Logger & l4 = Logger::instance(l1);
	EXPECT_EQ(&l1, &l4);
	Logger & l5 = Logger::instance();
	EXPECT_EQ(&l1, &l5);
}

}
}
}

#endif