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
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/types.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

bool complete_generic(Action & action, bool invert, int tries, Milliseconds delay, Logger::Level level) {
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
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			Multiplex multiplex(requests);
			multiplex.complete();
		}
		if (action.isRetryable()) {
			logger.log(level, "retrying %p\n", &action);
		} else if ((!invert) && (!action.isSuccessful())) {
			logger.log(level, "converging %p\n", &action);
		} else if (invert && (!action.isNonexistent())) {
			logger.log(level, "converging %p\n", &action);
		} else {
			break;
		}
		platform.yield(delay);
		action.start();
		continue;
	}
	return (action.isSuccessful() || (invert && action.isNonexistent()));
}

bool service_generic(Action & action, bool invert, int tries, Milliseconds delay, int iterations, Milliseconds timeout, Logger::Level level) {
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
	for (int ii = 0; ii < tries; ++ii) {
		requests = action.getRequests();
		if (requests != 0) {
			if (action == false) {
				Multiplex multiplex(requests);
				if (multiplex.service(timeout, iterations) <= 0) { break; }
				if (action == false) {
					logger.log(level, "timedout %p\n", &action);
					continue;
				}
			}
		}
		if (action.isRetryable()) {
			logger.log(level, "retrying %p\n", &action);
		} else if ((!invert) && (!action.isSuccessful())) {
			logger.log(level, "converging %p\n", &action);
		} else if (invert && (!action.isNonexistent())) {
			logger.log(level, "converging %p\n", &action);
		} else {
			break;
		}
		platform.yield(pause);
		action.start();
		continue;
	}
	return (action.isSuccessful() || (invert && action.isNonexistent()));
}

}
}
}
}
