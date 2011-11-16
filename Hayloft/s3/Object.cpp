/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Queue.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Object::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	::S3Status status = that->properties(properties);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	Logger::instance().log(level, "Object@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	show(properties, level);
	return status;
}

void Object::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	that->status = status;
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	Logger::instance().log(level, "Object@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	show(errorDetails, level);
	that->complete(status, errorDetails);
}

Object::Object(const Bucket & bucket, const char * keyname)
: id(bucket.getId())
, secret(bucket.getSecret())
, hostname(bucket.getHostName())
, name(bucket.getName())
, key(keyname)
, protocol(bucket.getProtocol())
, style(bucket.getStyle())
, requests(bucket.getRequests())
, status(::S3StatusOK)
{
	initialize();
}

Object::~Object() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}


void Object::initialize() {
	if (requests != 0) { Logger::instance().debug("Object@%p: requests=%p\n", this, requests); }
	std::memset(&context, 0, sizeof(context));
	context.hostName = hostname.c_str();
	context.bucketName = name.c_str();
	context.protocol = protocol;
	context.uriStyle = style;
	context.accessKeyId = id.c_str();
	context.secretAccessKey = secret.c_str();
	show(&context);
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
}

::S3Status Object::properties(const ::S3ResponseProperties * properties) {
	return ::S3StatusOK;
}

void Object::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	return;
}


}
}
}
}
