/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/convergence.h"
#include "com/diag/hayloft/s3/Action.h"
#include "com/diag/hayloft/s3/Plex.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Fibonacci.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/types.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

bool complete_generic(Action & action, bool converge, bool invert, int tries, Milliseconds delay, Logger::Level level) {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();
	ticks_t numerator;
	ticks_t denominator;
	platform.frequency(numerator, denominator);
	delay = ((delay * numerator) / denominator) / 1000;
	if (action.isIdle()) {
		action.start();
	}
	Fibonacci factor;
	::S3RequestContext * requests;
	int rc;
	Milliseconds effective;
	const char * label = "";
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			Plex plex(requests);
			rc = plex.complete();
			if ((rc & Plex::ERROR) != 0) {
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

bool service_generic(Action & action, bool converge, bool invert, int tries, Milliseconds delay, int iterations, Milliseconds timeout, Logger::Level level) {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	Logger & logger = Logger::instance();
	ticks_t numerator;
	ticks_t denominator;
	platform.frequency(numerator, denominator);
	ticks_t pause = ((delay * numerator) / denominator) / 1000;
	if (action.isIdle()) {
		action.start();
	}
	Fibonacci factor;
	::S3RequestContext * requests;
	int rc;
	Milliseconds effective;
	const char * label = "";
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			if (action.isBusy()) {
				Plex plex(requests);
				rc = plex.service(timeout, iterations);
				if ((rc & Plex::ERROR) != 0) {
					// Uh oh. No point in proceeding any further.
					logger.log(level, "failing@%p\n", &action);
					break;
				} else if ((rc & Plex::RETRY) != 0) {
					// Still uh oh. But libs3 thinks it might work if we RETRY.
					logger.log(level, "repeating@%p\n", &action);
					continue;
				} else if ((rc & Plex::PENDING) == 0) {
					// There are no PENDING Actions. Either all PENDING Actions
					// have completed, successfully or not, or something is
					// seriously amiss. Fall through to the checks below that
					// figure our what to do for this specific Action.
				} else if ((rc & Plex::READY) == 0) {
					// We have Actions PENDING but there is no READY responses
					// from S3. That work may be PENDING Actions other than the
					// only Action we care about. Selfish bastards we are.
					// Still, useful to note that we're waiting on S3 to
					// respond.
					logger.log(level, "waiting@%p\n", &action);
				} else {
					// We have PENDING Actions, and READY responses from S3.
					// That work may be PENDING Actions other than the only
					// Action we care about. Still selfish bastards.
				}
				if (action.isBusy()) {
					// This Action is still BUSY. Keep processing. We don't have
					// to code a delay here since the service() method itself
					// has a built-in delay.
					continue;
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
}
