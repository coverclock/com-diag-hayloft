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
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Bucket::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	Bucket * that = static_cast<Bucket*>(callbackData);
	::S3Status status = that->properties(properties);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	Logger::instance().log(level, "Bucket@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	show(properties, level);
	return status;
}

void Bucket::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	Bucket * that = static_cast<Bucket*>(callbackData);
	that->status = status;
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	Logger::instance().log(level, "Bucket@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	show(errorDetails, level);
	that->complete(status, errorDetails);
}

Bucket::Bucket(const char * bucketname, const Context & context, const Session & session)
: id(context.getId())
, secret(context.getSecret())
, hostname(session.getHostName())
, name(session.canonicalize(bucketname))
, region(context.getRegion())
, protocol(context.getProtocol())
, style(context.getStyle())
, access(context.getAccess())
, requests(0)
, status(::S3StatusOK)
{
	initialize();
}

Bucket::Bucket(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: id(context.getId())
, secret(context.getSecret())
, hostname(session.getHostName())
, name(session.canonicalize(bucketname))
, region(context.getRegion())
, protocol(context.getProtocol())
, style(context.getStyle())
, access(context.getAccess())
, requests(multiplex.getRequests())
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
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	logger.debug("Bucket@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
	logger.debug("Bucket@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
	logger.debug("Bucket@%p: hostname=\"%s\"\n", this, hostname.c_str());
	logger.debug("Bucket@%p: name=\"%s\"\n", this, name.c_str());
	logger.debug("Bucket@%p: region=\"%s\"\n", this, region.c_str());
	logger.debug("Bucket@%p: protocol=%d\n", this, protocol);
	logger.debug("Bucket@%p: style=%d\n", this, style);
	logger.debug("Bucket@%p: access=%d\n", this, access);
	if (requests != 0) { logger.debug("Bucket@%p: requests=%p\n", this, requests); }
}

::S3Status Bucket::getStatus(const char ** description) const {
	if (description != 0) { *description = ::S3_get_status_name(status); }
	return status;
}

::S3Status Bucket::properties(const ::S3ResponseProperties * properties) {
	return ::S3StatusOK;
}

void Bucket::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	return;
}

}
}
}
}
