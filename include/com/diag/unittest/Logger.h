/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdlib>
#include <cstdio>
#include "gtest/gtest.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Output.h"

namespace com {
namespace diag {
namespace hayloft {
namespace unittest {

TEST(LoggerTest, Defaults) {
	Logger logger;
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_FALSE(logger.isEnabled(logger.NOTICE));
	EXPECT_FALSE(logger.isEnabled(logger.WARNING));
	EXPECT_FALSE(logger.isEnabled(logger.ERROR));
	EXPECT_FALSE(logger.isEnabled(logger.SEVERE));
	EXPECT_FALSE(logger.isEnabled(logger.CRITICAL));
	EXPECT_FALSE(logger.isEnabled(logger.ALERT));
	EXPECT_FALSE(logger.isEnabled(logger.FATAL));
	EXPECT_FALSE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_FALSE(logger.isEnabled(logger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, logger.getMask());
}

TEST(LoggerTest, Instance) {
	Logger & logger = Logger::instance();
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_TRUE(logger.isEnabled(logger.NOTICE));
	EXPECT_TRUE(logger.isEnabled(logger.WARNING));
	EXPECT_TRUE(logger.isEnabled(logger.ERROR));
	EXPECT_TRUE(logger.isEnabled(logger.SEVERE));
	EXPECT_TRUE(logger.isEnabled(logger.CRITICAL));
	EXPECT_TRUE(logger.isEnabled(logger.ALERT));
	EXPECT_TRUE(logger.isEnabled(logger.FATAL));
	EXPECT_TRUE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_TRUE(logger.isEnabled(logger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0xff80, logger.getMask());
}

#define LOGGER_TEST_ENABLEDISABLE_SET(_LEVEL_) \
	do { \
		if ((mask & (1 << _LEVEL_)) != 0) { \
			logger.enable(_LEVEL_); \
		} else { \
			logger.disable(_LEVEL_); \
		} \
	} while (false)

#define LOGGER_TEST_ENABLEDISABLE_TEST(_LEVEL_) \
	do { \
		if ((mask & (1 << _LEVEL_)) != 0) { \
			EXPECT_TRUE(logger.isEnabled(_LEVEL_)); \
		} else { \
			EXPECT_FALSE(logger.isEnabled(_LEVEL_)); \
		} \
	} while (false)

TEST(LoggerTest, EnableDisable) {
	Logger logger;
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_FALSE(logger.isEnabled(logger.NOTICE));
	EXPECT_FALSE(logger.isEnabled(logger.WARNING));
	EXPECT_FALSE(logger.isEnabled(logger.ERROR));
	EXPECT_FALSE(logger.isEnabled(logger.SEVERE));
	EXPECT_FALSE(logger.isEnabled(logger.CRITICAL));
	EXPECT_FALSE(logger.isEnabled(logger.ALERT));
	EXPECT_FALSE(logger.isEnabled(logger.FATAL));
	EXPECT_FALSE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_FALSE(logger.isEnabled(logger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, logger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.PRINT);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, logger.getMask());
	}
}

TEST(LoggerTest, Initialization) {
	::com::diag::desperado::Output nulloutput;
	Logger logger;
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_FALSE(logger.isEnabled(logger.NOTICE));
	EXPECT_FALSE(logger.isEnabled(logger.WARNING));
	EXPECT_FALSE(logger.isEnabled(logger.ERROR));
	EXPECT_FALSE(logger.isEnabled(logger.SEVERE));
	EXPECT_FALSE(logger.isEnabled(logger.CRITICAL));
	EXPECT_FALSE(logger.isEnabled(logger.ALERT));
	EXPECT_FALSE(logger.isEnabled(logger.FATAL));
	EXPECT_FALSE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_FALSE(logger.isEnabled(logger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, logger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.PRINT);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.PRINT);
		/**/
		logger.setOutput(nulloutput);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, logger.getMask());
	}
}

TEST(LoggerTest, SetMask) {
	Logger logger;
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_FALSE(logger.isEnabled(logger.NOTICE));
	EXPECT_FALSE(logger.isEnabled(logger.WARNING));
	EXPECT_FALSE(logger.isEnabled(logger.ERROR));
	EXPECT_FALSE(logger.isEnabled(logger.SEVERE));
	EXPECT_FALSE(logger.isEnabled(logger.CRITICAL));
	EXPECT_FALSE(logger.isEnabled(logger.ALERT));
	EXPECT_FALSE(logger.isEnabled(logger.FATAL));
	EXPECT_FALSE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_FALSE(logger.isEnabled(logger.PRINT));
	EXPECT_EQ((Logger::Mask)0, logger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		logger.setMask(mask);
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, logger.getMask());
	}
}

TEST(LoggerTest, SetMaskEnvironment) {
	Logger logger;
	EXPECT_FALSE(logger.isEnabled(logger.FINEST));
	EXPECT_FALSE(logger.isEnabled(logger.FINER));
	EXPECT_FALSE(logger.isEnabled(logger.FINE));
	EXPECT_FALSE(logger.isEnabled(logger.TRACE));
	EXPECT_FALSE(logger.isEnabled(logger.DEBUG));
	EXPECT_FALSE(logger.isEnabled(logger.INFORMATION));
	EXPECT_FALSE(logger.isEnabled(logger.CONFIGURATION));
	EXPECT_FALSE(logger.isEnabled(logger.NOTICE));
	EXPECT_FALSE(logger.isEnabled(logger.WARNING));
	EXPECT_FALSE(logger.isEnabled(logger.ERROR));
	EXPECT_FALSE(logger.isEnabled(logger.SEVERE));
	EXPECT_FALSE(logger.isEnabled(logger.CRITICAL));
	EXPECT_FALSE(logger.isEnabled(logger.ALERT));
	EXPECT_FALSE(logger.isEnabled(logger.FATAL));
	EXPECT_FALSE(logger.isEnabled(logger.EMERGENCY));
	EXPECT_FALSE(logger.isEnabled(logger.PRINT));
	/**/
	EXPECT_EQ((Logger::Mask)0, logger.getMask());
	for (int mask = 0; mask <= 0xffff; ++mask) {
		char string[sizeof("0xffff")];
		std::snprintf(string, sizeof(string), "0x%x", mask);
		::setenv(logger.MASK_ENV_NAME(), string, !0);
		logger.setMask();
		/**/
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_TEST(logger.PRINT);
		/**/
		EXPECT_EQ((Logger::Mask)mask, logger.getMask());
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

TEST(LoggerTest, Logging) {
	Logger & logger = Logger::instance();
	for (int mask = 0; mask <= 0xffff; mask = (mask == 0) ? 1 : (mask << 1)) {
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINEST);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINER);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FINE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.TRACE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.DEBUG);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.INFORMATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CONFIGURATION);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.NOTICE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.WARNING);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ERROR);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.SEVERE);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.CRITICAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.ALERT);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.FATAL);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.EMERGENCY);
		LOGGER_TEST_ENABLEDISABLE_SET(logger.PRINT);
		/**/
		LOGGER_TEST_LOGGING_TEST(logger.FINEST,logger.finest("finest\n"));
		LOGGER_TEST_LOGGING_TEST(logger.FINER,logger.finer("finer\n"));
		LOGGER_TEST_LOGGING_TEST(logger.FINE,logger.fine("fine\n"));
		LOGGER_TEST_LOGGING_TEST(logger.TRACE,logger.trace("trace\n"));
		LOGGER_TEST_LOGGING_TEST(logger.DEBUG,logger.debug("debug\n"));
		LOGGER_TEST_LOGGING_TEST(logger.INFORMATION,logger.information("information\n"));
		LOGGER_TEST_LOGGING_TEST(logger.CONFIGURATION,logger.configuration("configuration\n"));
		LOGGER_TEST_LOGGING_TEST(logger.NOTICE,logger.notice("notice\n"));
		LOGGER_TEST_LOGGING_TEST(logger.WARNING,logger.warning("warning\n"));
		LOGGER_TEST_LOGGING_TEST(logger.ERROR,logger.error("error\n"));
		LOGGER_TEST_LOGGING_TEST(logger.SEVERE,logger.severe("severe\n"));
		LOGGER_TEST_LOGGING_TEST(logger.CRITICAL,logger.critical("critical\n"));
		LOGGER_TEST_LOGGING_TEST(logger.ALERT,logger.alert("alert\n"));
		LOGGER_TEST_LOGGING_TEST(logger.FATAL,logger.fatal("fatal\n"));
		LOGGER_TEST_LOGGING_TEST(logger.EMERGENCY,logger.emergency("emergency\n"));
		/**/
		EXPECT_TRUE(logger.print("print\n") > 0);
	}
}

}
}
}
}
