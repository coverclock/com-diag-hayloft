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

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Object::getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	return that->get(bufferSize, buffer);
}

int Object::putObjectDataCallback(int bufferSize, char * buffer, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	return that->put(bufferSize, buffer);
}

::S3Status Object::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	return that->properties(properties);
}

void Object::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	that->complete(status, errorDetails);
}

Object::Object(const Bucket & bucket, const char * keyname)
: hostname(bucket.getHostName())
, name(bucket.getName())
, protocol(bucket.getProtocol())
, style(bucket.getStyle())
, id(bucket.getId())
, secret(bucket.getSecret())
, requests(bucket.getRequests())
, key(keyname)
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
	Logger & logger = Logger::instance();
	logger.debug("Object@%p: hostname=\"%s\"\n", this, hostname.c_str());
	logger.debug("Object@%p: name=\"%s\"\n", this, name.c_str());
	logger.debug("Object@%p: protocol=%d\n", this, protocol);
	logger.debug("Object@%p: style=%d\n", this, style);
	logger.debug("Object@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
	logger.debug("Object@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
	logger.debug("Object@%p: requests=%p\n", this, requests);
	logger.debug("Object@%p: key=\"%s\"\n", this, key.c_str());
	if (key.length() == 0) {
		logger.warning("Object@%p: key too short! (%zu<1)\n", this, key.length());
	} else 	if (key.length() > LENGTH) {
		logger.warning("Object@%p: key too long! (%zu>%zu)\n", this, key.length(), LENGTH);
	} else {
		// Do nothing.
	}
}

::S3Status Object::get(int bufferSize, const char * buffer) {
	return ::S3StatusAbortedByCallback;
}

int Object::put(int bufferSize, char * buffer) {
	return -1;
}

::S3Status Object::properties(const ::S3ResponseProperties * properties) {
	show(properties, Logger::DEBUG);
}

void Object::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger & logger = Logger::instance();
	status = s3status;
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	logger.log(level, "Object@%p: status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	show(errorDetails, level);
}

}
}
}
}
