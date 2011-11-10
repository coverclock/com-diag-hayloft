/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Queue.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"
#include "com/diag/hayloft/s3/Error.h"
#include "com/diag/hayloft/s3/Response.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketCreate::BucketCreate(Session & se, const char * na, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(0)
, context(co)
, status(static_cast<S3Status>(BUSY)) // Why not static_cast<::S3Status>(BUSY)?
{
	run();
}

BucketCreate::BucketCreate(Session & se, const char * na, Queue & qu, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(qu.getRequests())
, context(co)
, status(static_cast<S3Status>(BUSY)) // Why not static_cast<::S3Status>(BUSY)?
{
	run();
}

BucketCreate::~BucketCreate() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketCreate::run() {
	Logger & logger = Logger::instance();
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	::S3_create_bucket(
		context.getProtocol(),
		context.getId(),
		context.getSecret(),
		session.getHostName(),
		name.c_str(),
		context.getList(),
		context.getConstraint(),
		requests,
		&handler,
		this
	);
	logger.debug("BucketTest@%p: name=\"%s\"\n", this, name.c_str());
	logger.debug("BucketTest@%p: requests=%p\n", this, requests);
}

::S3Status BucketCreate::properties(const ::S3ResponseProperties * properties) {
	show(properties, Logger::DEBUG);
}

void BucketCreate::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger & logger = Logger::instance();
	status = s3status;
	Logger::Level level = isCreated() ? Logger::DEBUG : Logger::ERROR;
	logger.log(level, "BucketCreate@%p: status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	show(errorDetails, level);
}

::S3Status BucketCreate::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	BucketCreate * that = static_cast<BucketCreate*>(callbackData);
	return that->properties(properties);
}

void BucketCreate::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	BucketCreate * that = static_cast<BucketCreate*>(callbackData);
	that->complete(status, errorDetails);
}

}
}
}
}
