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
#include "com/diag/hayloft/s3/Show.h"
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

Bucket::Bucket(const Session & session, const char * bucketname, const Context & context)
: hostname(session.getHostName())
, name(session.canonicalize(bucketname))
, requests(0)
, id(context.getId())
, secret(context.getSecret())
, region(context.getRegion())
, protocol(context.getProtocol())
, style(context.getStyle())
, access(context.getAccess())
, status(::S3StatusOK)
{
	initialize();
}

Bucket::Bucket(const Session & session, const char * bucketname, Queue & queue, const Context & context)
: hostname(session.getHostName())
, name(session.canonicalize(bucketname))
, requests(queue.getRequests())
, id(context.getId())
, secret(context.getSecret())
, region(context.getRegion())
, protocol(context.getProtocol())
, style(context.getStyle())
, access(context.getAccess())
, status(::S3StatusOK)
{
	initialize();
}

Bucket::~Bucket() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void Bucket::initialize() {
	Logger & logger = Logger::instance();
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	logger.debug("Bucket@%p: hostname=\"%s\"\n", this, hostname.c_str());
	logger.debug("Bucket@%p: name=\"%s\"\n", this, name.c_str());
	if (name.length() == 0) {
		logger.warning("Bucket@%p: name too short! (%zu<1)\n", this, name.length());
	} else 	if (name.length() > LENGTH) {
		logger.warning("Bucket@%p: name too long! (%zu>%zu)\n", this, name.length(), LENGTH);
	} else {
		// Do nothing.
	}
	logger.debug("Bucket@%p: requests=%p\n", this, requests);
	logger.debug("Bucket@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
	logger.debug("Bucket@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
	logger.debug("Bucket@%p: region=\"%s\"\n", this, region.c_str());
	logger.debug("Bucket@%p: protocol=%d\n", this, protocol);
	logger.debug("Bucket@%p: style=%d\n", this, style);
	logger.debug("Bucket@%p: access=%d\n", this, access);
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
