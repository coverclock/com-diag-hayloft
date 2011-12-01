/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CONVERGENCE_
#define _H_COM_DIAG_HAYLOFT_S3_CONVERGENCE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Action;

/**
 * Handle the retry and convergence logic for an Action by blocking until it is
 * complete. Both synchronous and asynchronous Actions are handled.
 *
 * @param action refers to the action.
 * @param converge if true causes the function to loop until the desired state
 *        is reached or tries is exhausted.
 * @param invert if true indicates the desired state is inverted (Non-Existence)
 *        otherwise it is not (Success).
 * @param tries is the limit on the total number of tries for either retryable
 *        failures or convergence.
 * @param delay is the number of milliseconds to delay between retries.
 * @param level is the logging level.
 * @return true if the desired state was achieved.
 */
extern bool complete_generic(Action & action, bool converge = true, bool invert = false, int tries = 20, Milliseconds delay = 1000, Logger::Level level = Logger::DEBUG);

/**
 * Complete until the state of the Action converges to any non-retryable state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true any non-retryable state was achieved.
 */
inline bool complete(Action & action, Logger::Level level = Logger::DEBUG) {
	return complete_generic(action, false, false, 20, 1000, level);
}

/**
 * Complete until the state of the Action converges to a Success state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true if a Success state was achieved.
 */
inline bool complete_until_success(Action & action, Logger::Level level = Logger::DEBUG) {
	return complete_generic(action, true, false, 20, 1000, level);
}

/**
 * Complete until the state of the Action converges to a Non-existence state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true if a Non-existence state was achieved.
 */
inline bool complete_until_nonexistent(Action & action, Logger::Level level = Logger::DEBUG) {
	return complete_generic(action, true, true, 20, 1000, level);
}

/**
 * Handle the retry and convergence logic for an Action by iteratively servicing
 * it until it is complete. Both synchronous and asynchronous Actions are
 * handled. When the Action is asynchronous, this mechanism is really only
 * useful when that Action is the only one on the Multiplex, since it only
 * checks the one Action for completion. Truly multiplexing multiple
 * asynchronous Actions will require a more application level approach. But
 * this function can serve as a model for such an implementation.
 *
 * @param action refers to the action.
 * @param converge if true causes the function to loop until the desired state
 *        is reached or tries is exhausted.
 * @param invert if true indicates the desired state is inverted (Non-Existence)
 *        otherwise it is not (Success).
 * @param tries is the limit on the total number of tries for either retryable
 *        failures or convergence.
 * @param delay is the number of milliseconds to delay between retries.
 * @oaram iterations is the limit on the number of iterations while servicing
 *        the socket collection (providing there is anything to do) before
 *        taking a delay.
 * @param timeout is the number of milliseconds to wait for any socket in the
 *        socket collection to become interesting.
 * @param level is the logging level.
 * @return true if the desired state was achieved.
 */
extern bool service_generic(Action & action, bool converge = true, bool invert = false, int tries = 20, Milliseconds delay = 1000, int iterations = 100, Milliseconds timeout = 100, Logger::Level level = Logger::DEBUG);

/**
 * Service until the state of the Action converges to any non-retryable state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true any non-retryable state was achieved.
 */
inline bool service(Action & action, Logger::Level level = Logger::DEBUG) {
	return service_generic(action, false, false, 20, 1000, 100, 1000, level);
}

/**
 * Service until the state of the Action converges to a Success state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true if a Success state was achieved.
 */
inline bool service_until_success(Action & action, Logger::Level level = Logger::DEBUG) {
	return service_generic(action, true, false, 20, 1000, 100, 1000, level);
}

/**
 * Service until the state of the Action converges to a Non-existence state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @param level is the logging level.
 * @return true if a Non-existence state was achieved.
 */
inline bool service_until_nonexistent(Action & action, Logger::Level level = Logger::DEBUG) {
	return service_generic(action, true, true, 20, 1000, 100, 1000, level);
}

}
}
}
}

#endif
