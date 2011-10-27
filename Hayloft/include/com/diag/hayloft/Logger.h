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

#include <new>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Platform.h"

namespace com {
namespace diag {
namespace hayloft {

class Logger : public ::com::diag::desperado::Logger {

public:

    /**
     * This mask holds a bit for every possible log level encoded as (1<<level).
     */
	typedef uint16_t Mask;

protected:

    /**
     * Points to the default instance of this object.
     */
    static Logger * singleton;

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
     * Returns the name of the environmental variable that can be used to
     * set the mask.
     */
    static const char * MASK_ENV_NAME() {
    	return "COM_DIAG_HAYLOFT_LOGGER_MASK";
    }

	/**
	 * Ctor.
	 */
    explicit Logger()
    : ::com::diag::desperado::Logger(::com::diag::desperado::Platform::instance().log())
    , mask(0)
    {
    }

    /**
     * Dtor.
     * Any buffered output is flushed.
     */
    virtual ~Logger() {
    	(getOutput())();
    }

    /**
     * Set the Output functor to which log messages are emitted.
	 * @param ro refers to an Output functor to which log messages are emitted.
     * @return a reference to this object.
     */
    Logger & setOutput(::com::diag::desperado::Output & ro) {
    	::com::diag::desperado::Logger logger(ro);
    	// Yes, I know this is kinda funky. We just want to reinitialize the
    	// super class portion of ourselves with the new version to set the
    	// output functor, and the super class doesn't have an operation with
    	// which to do that.
    	// IMO this is legal and should compile, but does not. Worse, the
    	// diagnostics are completely mysterious.
    	//  static_cast<::com::diag::desperado::Logger&>(*this) = logger;
    	// Much to my surprise, this compiles and works.
    	//  ::com::diag::desperado::Logger::operator=(logger);
    	// But this is a little less scary looking, and works fine.
    	::com::diag::desperado::Logger& that = *this;
    	that = logger;
    	return *this;
    }

    /**
     * Get a reference to the current output functor.
     * @return the current output functor.
     */
    ::com::diag::desperado::Output & getOutput() {
    	return ::com::diag::desperado::Logger::output();
    }

    /**
     * Set the mask which controls which log levels are enabled.
     * @param vm is the new mask value.
     * @return a reference to this object.
     */
    Logger & setMask(Mask vm) {
    	mask = vm;
    	return *this;
    }

    /**
     * Set the mask which controls which log levels are enabled from
     * an environmental variable.
     * @return a reference to this object.
     */
    Logger & setMask();

    /**
     * Get the current mask. This is mostly useful for saving the current mask
     * so that it can be restored after temporarily changing it.
     * @return the current mask;
     */
    Mask getMask() {
    	return mask;
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

    /**
     * Returns true if the specified log level is enabled, false otherwise.
     * @return true if the specified log level is enabled, false otherwise.
     */
    virtual bool isEnabled(Level level) {
    	return (mask & ((Mask)1 << level)) != 0;
    }

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

};

}
}
}

#endif
