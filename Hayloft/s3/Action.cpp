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
	Status status = that->properties(responseProperties);
	// Not safe to reference object fields or methods after this point.
	// Application is permitted to delete object.
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	Logger::instance().log(level, "Action@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	show(responseProperties, level);
	return status;
}

void Action::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Action * that = static_cast<Action*>(callbackData);
	that->status = status;
	// An ::S3StatusInterrupted means someone destroyed our request context.
	// We are now a synchronous Action.
	if (status == ::S3StatusInterrupted) { that->pending = 0; }
	Logger::Level level;
	// I've never seen ::S3StatusErrorNoSuchKey in response to a GET or a HEAD
	// so I'm not sure under what circumstances it occurs. A missing OBJECT
	// seems to result in the typical HTTP error NOT FOUND a.k.a. the dreaded
	// "404".
	switch (status) {
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
	Logger::instance().log(level, "Action@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	show(errorDetails, level);
	that->complete(errorDetails);
	// Not safe to reference object fields or methods after this point.
	// Application is permitted to delete object.
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
	LifeCycle::instance().destructor(*this);
}

void Action::initialize() {
	if (pending != 0) { Logger::instance().debug("Action@%p: pending=%p\n", this, pending); }
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	LifeCycle::instance().constructor(*this);
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

Status Action::properties(const ::S3ResponseProperties * properties) {
	LifeCycle::instance().properties(*this, properties);
	return ::S3StatusOK;
}

void Action::complete(const ::S3ErrorDetails * errorDetails) {
	LifeCycle::instance().complete(*this, errorDetails);
}

}
}
}
}
