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
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/tostring.h"
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
	::S3RequestContext * requests;
	int rc;
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			Multiplex multiplex(requests);
			rc = multiplex.complete();
			if ((rc & Multiplex::ERROR) != 0) {
				logger.log(level, "failing@%p\n", &action);
				break;
			}
		}
		if (action.isRetryable()) {
			logger.log(level, "retrying@%p\n", &action);
		} else if (converge && (!invert) && (!action.isSuccessful())) {
			logger.log(level, "converging@%p\n", &action);
		} else if (converge && invert && (!action.isNonexistent())) {
			logger.log(level, "converging@%p\n", &action);
		} else {
			break;
		}
		platform.yield(delay);
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
	::S3RequestContext * requests;
	int rc;
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			if (action.isBusy()) {
				Multiplex multiplex(requests);
				rc = multiplex.service(timeout, iterations);
				if ((rc & Multiplex::ERROR) != 0) {
					// Uh oh. No point in proceeding any further.
					logger.log(level, "failing@%p\n", &action);
					break;
				} else if ((rc & Multiplex::RETRY) != 0) {
					// Uh oh. But libs3 thinks it might work if we RETRY.
					logger.log(level, "repeating@%p\n", &action);
					continue;
				} else if ((rc & Multiplex::PENDING) == 0) {
					// There are no PENDING Actions. Either all PENDING Actions
					// have completed, successfully or not, or something is
					// seriously amiss. Fall through to the checks below that
					// figure our what to do for this specific Action.
				} else if ((rc & Multiplex::READY) == 0) {
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
					// This Action still isn't complete. Keep processing.
					continue;
				}
			}
		}
		if (action.isBusy()) {
			logger.log(level, "hanging@%p\n", &action);
			break;
		} else if (action.isRetryable()) {
			logger.log(level, "retrying@%p\n", &action);
		} else if (converge && (!invert) && (!action.isSuccessful())) {
			logger.log(level, "converging@%p\n", &action);
		} else if (converge && invert && (!action.isNonexistent())) {
			logger.log(level, "converging@%p\n", &action);
		} else {
			break;
		}
		platform.yield(pause);
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
