/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/lariat/lariat.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/LogOutput.h"

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
	for (int mask = 0; mask < 0x10000; ++mask) {
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
		//
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
	}
}

TEST(LoggerTest, Logging) {
	::com::diag::desperado::LogOutput output(::com::diag::desperado::Platform::instance().error());
    Logger logger;
	logger.setOutput(output);
	logger.enable(logger.FINEST);
	logger.enable(logger.FINER);
	logger.enable(logger.FINE);
	logger.enable(logger.TRACE);
	logger.enable(logger.DEBUG);
	logger.enable(logger.INFORMATION);
	logger.enable(logger.CONFIGURATION);
	logger.enable(logger.NOTICE);
	logger.enable(logger.WARNING);
	logger.enable(logger.ERROR);
	logger.enable(logger.SEVERE);
	logger.enable(logger.CRITICAL);
	logger.enable(logger.ALERT);
	logger.enable(logger.FATAL);
	logger.enable(logger.EMERGENCY);
	logger.enable(logger.PRINT);
	logger.finest("finest\n");
	logger.finer("finer\n");
	logger.fine("fine\n");
	logger.trace("trace\n");
	logger.debug("debug\n");
	logger.information("information\n");
	logger.configuration("configuration\n");
	logger.notice("notice\n");
	logger.warning("warning\n");
	logger.error("error\n");
	logger.severe("severe\n");
	logger.critical("critical\n");
	logger.alert("alert\n");
	logger.fatal("fatal\n");
	logger.emergency("emergency\n");
	logger.print("print\n");
}

}
}
}
}
