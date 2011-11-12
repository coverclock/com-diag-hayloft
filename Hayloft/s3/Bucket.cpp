/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Queue.h"
#include "com/diag/hayloft/s3/Error.h"
#include "com/diag/hayloft/s3/Response.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Bucket::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	Bucket * that = static_cast<Bucket*>(callbackData);
	return that->properties(properties);
}

void Bucket::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Bucket * that = static_cast<Bucket*>(callbackData);
	that->complete(status, errorDetails);
}

Bucket::Bucket(const Session & se, const char * na, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(0)
, context(co)
, status(::S3StatusOK)
{
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
}

Bucket::Bucket(const Session & se, const char * na, Queue & qu, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(qu.getRequests())
, context(co)
, status(::S3StatusOK)
{
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
}

Bucket::~Bucket() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

::S3Status Bucket::properties(const ::S3ResponseProperties * properties) {
	show(properties, Logger::DEBUG);
}

void Bucket::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger & logger = Logger::instance();
	status = s3status;
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	logger.log(level, "Bucket@%p: status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	show(errorDetails, level);
}

}
}
}
}
