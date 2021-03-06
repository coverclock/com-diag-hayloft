/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_CONVERGENCE_
#define _H_COM_DIAG_HAYLOFT_CONVERGENCE_

/**
 * @file
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 *
 * These functions represent my attempts to figure out the error recovery and
 * consistency convergence logic of dealing with S3. A real application
 * architecture will probably choose to start many (possible unrelated)
 * asynchronous Actions in parallel on a single Plex and handle all of
 * those Actions in parallel, possibly doing so through a single service
 * thread. Rather than polling for completion, an application will probably
 * derive from the various Action classes and override the complete() method
 * or use the conditional variables with wait and signal. In particular see the
 * Complex Plex and its associated unit tests, which is how I think an actual
 * application will use Hayloft.
 */

#include "com/diag/hayloft/types.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/generics.h"

namespace com {
namespace diag {
namespace hayloft {

class Action;

namespace convergence {

static const int TRIES = 30;

static const Milliseconds DELAY = 20000; // 20 seconds.

static const int ITERATIONS = intmaxof(int); // Big.

static const Milliseconds TIMEOUT = 60000; // 60 seconds.

static const ::com::diag::grandote::MaskableLogger::Level LEVEL = ::com::diag::grandote::MaskableLogger::INFORMATION;

}

/**
 * Handle the error recovery and consistency convergence logic for an Action
 * by blocking until it is complete. Both synchronous and asynchronous Actions
 * are handled.
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
extern bool complete_generic(Action & action, bool converge, bool invert, int tries = convergence::TRIES, Milliseconds delay = convergence::DELAY, ::com::diag::grandote::MaskableLogger::Level level = convergence::LEVEL);

/**
 * Complete until the state of this Action converges to any non-retryable state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @return true any non-retryable state was achieved.
 */
inline bool complete(Action & action) {
	return complete_generic(action, false, false);
}

/**
 * Complete until the state of this Action converges to a Success state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @return true if a Success state was achieved.
 */
inline bool complete_until_successful(Action & action) {
	return complete_generic(action, true, false);
}

/**
 * Complete until the state of this Action converges to a Non-existence state.
 *
 * See complete_generic.
 *
 * @param action refers to the action.
 * @return true if a Non-existence state was achieved.
 */
inline bool complete_until_nonexistent(Action & action) {
	return complete_generic(action, true, true);
}

/**
 * Handle the error recovery and consistency convergence logic for an Action
 * by iteratively servicing it until it is complete. Both synchronous and
 * asynchronous Actions are handled. When this Action is asynchronous, this
 * mechanism is really only useful when that Action is the only one on the
 * Plex, since it only checks the one Action for completion. Truly
 * plexing multiple asynchronous Actions will require a more application
 * level approach. But this function can serve as a model for such an
 * implementation.
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
extern bool service_generic(Action & action, bool converge, bool invert, int tries = convergence::TRIES, Milliseconds delay = convergence::DELAY, int iterations = convergence::ITERATIONS, Milliseconds timeout = convergence::TIMEOUT, ::com::diag::grandote::MaskableLogger::Level level = convergence::LEVEL);

/**
 * Service until the state of this Action converges to any non-retryable state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @return true any non-retryable state was achieved.
 */
inline bool service(Action & action) {
	return service_generic(action, false, false);
}

/**
 * Service until the state of this Action converges to a Success state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @return true if a Success state was achieved.
 */
inline bool service_until_successful(Action & action) {
	return service_generic(action, true, false);
}

/**
 * Service until the state of this Action converges to a Non-existence state.
 *
 * See service_generic.
 *
 * @param action refers to the action.
 * @return true if a Non-existence state was achieved.
 */
inline bool service_until_nonexistent(Action & action) {
	return service_generic(action, true, true);
}

}
}
}

#endif
