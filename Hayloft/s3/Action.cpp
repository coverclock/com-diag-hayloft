/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Action.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/s3/Plex.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/CriticalSection.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Status Action::responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData) {
	Action * that = static_cast<Action*>(callbackData);
	if ((responseProperties->requestId != 0) && (responseProperties->requestId[0] != '\0')) { that->requestid = responseProperties->requestId; }
	if ((responseProperties->requestId2 != 0) && (responseProperties->requestId2[0] != '\0')) { that->requestid2 = responseProperties->requestId2; }
	if ((responseProperties->server != 0) && (responseProperties->server[0] != '\0')) { that->server = responseProperties->server; }
	Status status = LifeCycle::instance().properties(*that, responseProperties);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	Logger::instance().log(level, "Action@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	show(responseProperties, level);
	return status;
}

void Action::responseCompleteCallback(Status final, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Action * that = static_cast<Action*>(callbackData);
	// An ::S3StatusInterrupted means someone destroyed our request context.
	// We are henceforth a synchronous Action.
	if (final == ::S3StatusInterrupted) {
		that->pending = 0;
	}
	// I've never seen ::S3StatusErrorNoSuchKey in response to a GET or a HEAD
	// so I'm not sure under what circumstances it occurs. A missing OBJECT
	// seems to result in the typical HTTP error NOT FOUND a.k.a. the dreaded
	// "404".
	Logger::Level level;
	switch (final) {
	case ::S3StatusOK:
	case ::S3StatusErrorNoSuchBucket:
	case ::S3StatusErrorNoSuchKey:
	case ::S3StatusHttpErrorNotFound:
		level = Logger::DEBUG; // Considered to be an application issue.
		break;
	default:
		level = Logger::NOTICE; // Considered to be a platform issue.
		break;
	}
	Logger::instance().log(level, "Action@%p: status=%d=\"%s\"\n", that, final, tostring(final));
	show(errorDetails, level);
	// This is a transitional state after the Action has completed and before
	// the final state is assigned by the application. This provides a state
	// from which the application can safely delete or re-start the Action but
	// from which Action completion is not visible to other threads polling on
	// the status of the Action.
	that->state(static_cast<Status>(PENDING));
	// The application is permitted to delete or re-start the Action in the
	// LifeCycle method or in the Action method called by the LifeCycle
	// method.
	LifeCycle::instance().complete(*that, final, errorDetails);
	// Not safe to reference object fields or methods after this point since
	// the application is allowed to have deleted the Action.
	Logger::instance().log(level, "Action@%p: end\n", that);
}

Action::Action()
: pending(0)
, status(::S3StatusOK)
{
	initialize();
}

Action::Action(const Plex & plex)
: pending(plex.getPending())
, status(::S3StatusOK)
{
	initialize();
}

Action::~Action() {
	// We can't call S3_runall_request_context() from here because the libs3
	// thread will be accessing data in the derived class whose destructor has
	// already been invoked. Only those derived classes which actually implement
	// a start() method that enqueues a request on the request context should
	// call S3_runall_request_context() in their destructor.
	condition.signal();
	LifeCycle::instance().destructor(*this);
}

void Action::initialize() {
	if (pending != 0) {
		Logger::instance().debug("Action@%p: pending=%p\n", this, pending);
	}
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	LifeCycle::instance().constructor(*this);
}

void Action::execute() {
	LifeCycle::instance().start(*this);
}

Status Action::getStatus(const char ** description) const {
	if (description != 0) { *description = tostring(status); }
	return status;
}

void Action::start() {
	// It is the responsibility of the derived classes to call the
	// corresponding LifeCycle method when they actually start. This isn't
	// always done in their overriding start method but instead in a non-virtual
	// method, typically called execute, that may be called from their
	// constructor for the synchronous interface.
}

void Action::reset() {
}

Status Action::properties(const ::S3ResponseProperties * properties) {
	return ::S3StatusOK;
}

void Action::complete(Status final, const ::S3ErrorDetails * errorDetails) {
	// We wait to after we call the complete handler to store the status. That
	// allows the complete handler to see the Action has completed before any
	// other threads polling on the status will notice it.
	state(final);
}

Status Action::state() const {
	CriticalSection guard(mutex);
	return status;
}

Status Action::state(Status update) {
	CriticalSection guard(mutex);
	Status previous = status;
	status = update;
	if ((status != IDLE) && (status != BUSY) && (status != PENDING)) {
		condition.signal();
	}
	return previous;
}

int Action::wait() {
	CriticalSection guard(mutex);
	int rc = 0;
	while ((rc == 0) && (status != IDLE) && (status != BUSY) && (status != PENDING)) {
		rc = condition.wait(mutex);
	}
	return rc;
}

}
}
}
}
