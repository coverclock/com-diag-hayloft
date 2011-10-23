/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_HAYLOFT_LOGGER_H_
#define _COM_DIAG_HAYLOFT_LOGGER_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdint>
#include "com/diag/desperado/Logger.h"

namespace com {
namespace diag {
namespace hayloft {

class Logger : public ::com::diag::desperado::Logger {

protected:

    /**
     * Points to the default instance of this object.
     */
    static Logger * singleton;

    /**
     * This mask holds a bit for every possible log level encoded as (1<<level).
     */
	typedef uint16_t Mask;

    /**
     * Encodes the log mask where each bit indicates whether the log level
     * is enabled or not, encoded as (1 << level).
     */
    Mask mask;

public:

	/**
	 * Allocates a reference to a new object of this type suitably initialized
	 * with default parameters.
	 * @return a reference to a new object of this type.
	 */
	static Logger & factory();

	/**
	 * Establishes a new default instance of an object of this type.
	 * @param that refers to an object of this type.
	 */
	static void instance(Logger & that) {
		Logger::singleton = &that;
	}

    /**
     * Returns a reference to the default instance of an object of this type.
     * @return a reference to the default instance of an object of this type.
     */
	static Logger & instance() {
	    return *Logger::singleton;
	}

	/**
	 * Ctor.
	 * @param po points to an Output object to which log messages are emitted.
	 */
    explicit Logger(::com::diag::desperado::Output * po = 0)
    : ::com::diag::desperado::Logger(po)
    , mask(0)
    {
    }

	/**
	 * Ctor.
	 * @param ro refers to an Output object to which log messages are emitted.
	 */
    explicit Logger(::com::diag::desperado::Output& ro)
    : ::com::diag::desperado::Logger(ro)
    , mask(0)
    {
    }

    /**
     * Dtor. Any buffered output is flushed.
     */
    virtual ~Logger() {
    	(output())();
    }

    /**
     * Returns true if the specified log level is enabled, false otherwise.
     * @return true if the specified log level is enabled, false otherwise.
     */
    virtual bool isEnabled(Level level) {
    	return (mask & ((Mask)1 << level)) != 0;
    }

    /**
     * Enable the specified log level.
     * @param level is the log level to be enabled.
     * @return a reference to this object.
     */
    Logger & enable(Level level) {
    	mask |= ((Mask)1 << level);
    	return *this;
    }

    /**
     * Disable the specified log level.
     * @param level is the log level to be disabled.
     * @return a reference to this object.
     */
    Logger & disable(Level level) {
    	mask &= ~((Mask)1 << level);
    	return *this;
    }

    using ::com::diag::desperado::Logger::output;

    using ::com::diag::desperado::Logger::log;

    using ::com::diag::desperado::Logger::finest;

    using ::com::diag::desperado::Logger::finer;

    using ::com::diag::desperado::Logger::fine;

    using ::com::diag::desperado::Logger::trace;

    using ::com::diag::desperado::Logger::debug;

    using ::com::diag::desperado::Logger::information;

    using ::com::diag::desperado::Logger::configuration;

    using ::com::diag::desperado::Logger::notice;

    using ::com::diag::desperado::Logger::warning;

    using ::com::diag::desperado::Logger::error;

    using ::com::diag::desperado::Logger::severe;

    using ::com::diag::desperado::Logger::critical;

    using ::com::diag::desperado::Logger::alert;

    using ::com::diag::desperado::Logger::fatal;

    using ::com::diag::desperado::Logger::emergency;

    using ::com::diag::desperado::Logger::print;

private:

    /**
     * Copying is disabled.
     */
    Logger(const Logger& that);

    /**
     * Assignment is disabled.
     */
    Logger& operator=(const Logger& that);

};

}
}
}

#endif
