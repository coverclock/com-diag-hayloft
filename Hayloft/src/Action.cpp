/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Action.h"
#include "com/diag/hayloft/LifeCycle.h"
#include "com/diag/hayloft/Plex.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/CriticalSection.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {

bool Action::dontcare = false;

/*******************************************************************************
 * CALLBACK METHODS FOR LIBS3
 ******************************************************************************/

Status Action::responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData) {
	Logger & logger = Logger::instance();
	Action * that = static_cast<Action*>(callbackData);
	if ((responseProperties->requestId != 0) && (responseProperties->requestId[0] != '\0')) { that->requestid = responseProperties->requestId; }
	if ((responseProperties->requestId2 != 0) && (responseProperties->requestId2[0] != '\0')) { that->requestid2 = responseProperties->requestId2; }
	if ((responseProperties->server != 0) && (responseProperties->server[0] != '\0')) { that->server = responseProperties->server; }
	Status status = LifeCycle::instance().properties(*that, responseProperties);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	logger.log(level, "Action@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	show(responseProperties, level);
	return status;
}

void Action::responseCompleteCallback(Status final, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Logger & logger = Logger::instance();
	Action * that = static_cast<Action*>(callbackData);
	// An ::S3StatusInterrupted means someone destroyed our request context,
	// a (to us anyway) opaque handle that is used by libs3 and libcurl to
	// manage active Actions when using the asynchronous interface. Typically
	// this occurs when the Plex used when we were constructed is deleted while
	// we were busy. We are henceforth a synchronous Action. Wackiness ensues.
	if (final == ::S3StatusInterrupted) {
		Logger::instance().error("Action@%p: interrupted while busy!\n", that);
		that->handle = 0;
	}
	Logger::Level level;
	switch (final) {
	case ::S3StatusOK:
	case ::S3StatusErrorNoSuchBucket:
	case ::S3StatusErrorNoSuchKey: // e.g. ObjectCopy
	case ::S3StatusHttpErrorNotFound: // a.k.a. 404 e.g. ObjectHead, ObjectGet
		level = Logger::DEBUG; // Considered to be an application issue.
		break;
	default:
		level = Logger::NOTICE; // Considered to be a platform issue.
		break;
	}
	logger.log(level, "Action@%p: status=%d=\"%s\"\n", that, final, tostring(final));
	show(errorDetails, level);
	logger.debug("Action@%p: end\n", that);
	// The application is permitted to delete or re-start the Action in the
	// LifeCycle method or in the Action method called by the LifeCycle
	// method.
	LifeCycle::instance().complete(*that, final, errorDetails);
	// Not safe to reference object fields or methods after this point since
	// the application may have deleted the Action.
}

/*******************************************************************************
 * APPLICATION API
 ******************************************************************************/

Action::Action()
: handle(0)
, retries(0)
, status(::S3StatusOK)
{
	initialize();
}

Action::Action(const Plex & plex)
: handle(plex.getHandle())
, retries(0)
, status(::S3StatusOK)
{
	initialize();
}

Action::~Action() {
	// We shouldn't call S3_runall_request_context from here because the derived
	// class destructor has already been invoked. But this is simpler, and if
	// we're deleting this Action while it is busy, wackiness has already
	// ensued. This will force this Action to complete, and doing so will also
	// signal any waiting Threads if the Application is using a multi-threaded
	// Plex. Valgrind will have a stroke over this, I wager.
	if (isBusy()) {
		Logger::instance().error("Action@%p: deleted while busy!\n", this);
		if (handle != 0) { (void)S3_runall_request_context(handle); }
	}
	LifeCycle::instance().destructor(*this);
}

Status Action::getStatus(const char ** description) const {
	Status status = state();
	if (description != 0) { *description = tostring(status); }
	return status;
}

bool Action::start() {
	return (state() != BUSY);
}

bool Action::reset() {
	return (state() != BUSY);
}

bool Action::wait(Handle * candidate) {
	CriticalSection guard(mutex);
	if ((candidate != 0) && (handle != candidate)) {
		return false;
	} else {
		while ((status == PENDING) || (status == BUSY) || (status == FINAL)) { // IDLE doesn't wait.
			if (condition.wait(mutex) != 0) {
				return false;
			}
		}
		return true;
	}
}

/*******************************************************************************
 * MANAGEMENT API
 ******************************************************************************/

Status Action::state() const {
	CriticalSection guard(mutex);
	return status;
}

Status Action::state(Status update) {
	CriticalSection guard(mutex);
	Status previous = status;
	status = update;
	return previous;
}

bool Action::retryable(Status status, bool nonexistence) {
	bool result;
	switch (status) {
	case ::S3StatusHttpErrorNotFound:
	case ::S3StatusErrorNoSuchKey:
	case ::S3StatusErrorNoSuchBucket:
		result = nonexistence;
		break;
	default:
		result = ::S3_status_is_retryable(status);
		break;
	}
	return result;
}

bool Action::startable(Handle * candidate) {
	CriticalSection guard(mutex);
	Logger & logger = Logger::instance();
	bool result = false;
	if ((status == PENDING) || (status == BUSY) || (status == FINAL)) {
		logger.error("Action@%p: busy!\n", this); // Application error.
	} else if ((candidate != 0) && (handle != candidate)) {
		logger.error("Action@%p: wrong handle!\n", this); // Application error.
	} else {
		logger.debug("Action@%p: startable\n", this);
		status = static_cast<Status>(PENDING);
		retries = RETRIES;
		result = true;
	}
	return result;
}

bool Action::restartable(Status final, bool & unretryable, Handle * candidate) {
	CriticalSection guard(mutex);
	Logger & logger = Logger::instance();
	bool result = false;
	status = static_cast<Status>(FINAL);
	bool clue;
	if ((clue = !retryable(final))) {
		logger.debug("Action@%p: not retryable\n", this);
	} else if ((candidate != 0) && (handle != candidate)) {
		logger.debug("Action@%p: wrong handle\n", this);
	} else if (retries <= 0) {
		logger.debug("Action@%p: too many retries\n", this);
	} else if (!reset()) {
		logger.debug("Action@%p: not resettable\n", this);
	} else {
		logger.debug("Action@%p: restartable\n", this);
		status = static_cast<Status>(PENDING);
		--retries;
		result = true;
	}
	unretryable = clue;
	return result;
}

bool Action::signal(Status final) {
	CriticalSection guard(mutex);
	status = final;
	return (condition.signal() == 0);
}

/*******************************************************************************
 * STUBS THAT A SUBCLASS MAY OVERRIDE
 ******************************************************************************/

Status Action::properties(const ::S3ResponseProperties * properties) {
	return ::S3StatusOK;
}

void Action::complete(Status final, const ::S3ErrorDetails * errorDetails) {
}

/*******************************************************************************
 * UTILITIES
 ******************************************************************************/

void Action::initialize() {
	if (handle != 0) { Logger::instance().debug("Action@%p: handle=%p\n", this, handle); }
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	LifeCycle::instance().constructor(*this);
}

void Action::execute() {
	LifeCycle::instance().start(*this);
}

}
}
}
