/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/convergence.h"
#include "com/diag/hayloft/Action.h"
#include "com/diag/hayloft/Multiplex.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/Fibonacci.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/types.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

bool complete_generic(Action & action, bool converge, bool invert, int tries, Milliseconds delay, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::Platform & platform = ::com::diag::grandote::Platform::instance();
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	::com::diag::grandote::ticks_t numerator;
	::com::diag::grandote::ticks_t denominator;
	platform.frequency(numerator, denominator);
	delay = ((delay * numerator) / denominator) / 1000;
	if (action.isIdle()) {
		action.start();
	}
	::com::diag::grandote::Fibonacci factor;
	Handle * handle;
	int rc;
	Milliseconds effective;
	const char * label = "";
	for (int ii = 0; ii < tries; ++ii) {
		handle = action.getHandle();
		if (handle != 0) {
			Multiplex multiplex(handle);
			rc = multiplex.complete();
			if ((rc & Multiplex::ERROR) != 0) {
				logger.log(level, "failing@%p\n", &action);
				break;
			}
		}
		if (action.isRetryable()) {
			label = "retrying";
		} else if (converge && (!invert) && (!action.isSuccessful())) {
			label = "converging";
		} else if (converge && invert && (!action.isNonexistent())) {
			label = "converging";
		} else {
			break;
		}
		effective = factor * delay;
		logger.log(level, "%s@%p: %lldms\n", label, &action, effective);
		platform.yield(effective);
		action.start();
		continue;
	}
	bool result = ((!action.isBusy()) && (!action.isRetryable()) && ((!converge) || action.isSuccessful() || (invert && action.isNonexistent())));
	if (!result) {
		logger.log(level, "status@%p=%d=\"%s\"\n", &action, action.getStatus(), tostring(action.getStatus()));
	}
	return result;
}

bool service_generic(Action & action, bool converge, bool invert, int tries, Milliseconds delay, int iterations, Milliseconds timeout, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::Platform & platform = ::com::diag::grandote::Platform::instance();
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	::com::diag::grandote::ticks_t numerator;
	::com::diag::grandote::ticks_t denominator;
	platform.frequency(numerator, denominator);
	::com::diag::grandote::ticks_t pause = ((delay * numerator) / denominator) / 1000;
	if (action.isIdle()) {
		action.start();
	}
	::com::diag::grandote::Fibonacci factor;
	Handle * handle;
	int rc;
	Milliseconds effective;
	const char * label = "";
	for (int ii = 0; ii < tries; ++ii) {
		handle = action.getHandle();
		if (handle != 0) {
			if (action.isBusy()) {
				Multiplex multiplex(handle);
				rc = multiplex.service(timeout, iterations);
				if ((rc & Multiplex::ERROR) != 0) {
					// Uh oh. No point in proceeding any further.
					logger.log(level, "failing@%p\n", &action);
					break;
				} else if ((rc & Multiplex::RETRY) != 0) {
					// Still uh oh. But libs3 thinks it might work if we try
                    // again..
					logger.log(level, "repeating@%p\n", &action);
					continue;
				} else if ((rc & Multiplex::READY) != 0) {
                    // There are PENDING Actions *and* sockets with data
                    // READY to be read.
                    continue;
				} else if ((rc & Multiplex::PENDING) == 0) {
					// There are no PENDING Actions nor are there any open
                    // sockets at all. Not only is there nothing to do,
                    // there is no future possibility of anything to do.
                    // Fall through.
				} else if (action.isBusy()) {
					// This Action is still BUSY. Keep processing.
					logger.log(level, "iterating@%p\n", &action);
                    platform.yield();
					continue;
				} else {
                    // This Action is not BUSY. Fall through.
                }
			}
		}
		// It should be impossible for the Action to be BUSY at this point.
		// If it hasn't completed yet, then we're burned up our tries by
		// continuing above and never reach here.
		if (action.isRetryable()) {
			// The Action completed but unsuccessfully with a status that libs3
			// thinks might work if we try it again.
			label = "retrying";
		} else if (converge && (!invert) && (!action.isSuccessful())) {
			// The Action completed but hasn't yet converged to Success.
			// The application thinks it might if we try again.
			label = "converging";
		} else if (converge && invert && (!action.isNonexistent())) {
			// The Action completed but hasn't yet converged to Non-existence.
			// The application thinks it might if we try again.
			label = "converging";
		} else {
			// We're done.
			break;
		}
		effective = factor * pause;
		logger.log(level, "%s@%p: %lldms\n", label, &action, effective);
		platform.yield(effective);
		action.start();
		continue;
	}
	bool result = ((!action.isBusy()) && (!action.isRetryable()) && ((!converge) || action.isSuccessful() || (invert && action.isNonexistent())));
	if (!result) {
		logger.log(level, "status@%p=%d=\"%s\"\n", &action, action.getStatus(), tostring(action.getStatus()));
	}
	return result;
}

}
}
}
