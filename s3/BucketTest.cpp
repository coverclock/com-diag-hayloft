/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketTest.h"
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

BucketTest::BucketTest(Session & se, const char * na, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(0)
, context(co)
, status(static_cast<S3Status>(BUSY)) // Why not static_cast<::S3Status>(BUSY)?
{
	run();
}

BucketTest::BucketTest(Session & se, const char * na, Queue & qu, const Context & co)
: session(se)
, name(se.canonicalize(na))
, requests(qu.getRequests())
, context(co)
, status(static_cast<S3Status>(BUSY)) // Why not static_cast<::S3Status>(BUSY)?
{
	run();
}

BucketTest::~BucketTest() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketTest::run() {
	Logger & logger = Logger::instance();
	constraint[0] = '\0';
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	::S3_test_bucket(
		context.getProtocol(),
		context.getStyle(),
		context.getId(),
		context.getSecret(),
		session.getHostName(),
		name.c_str(),
		sizeof(constraint), constraint,
		requests,
		&handler,
		this
	);
	logger.debug("BucketTest@%p: requests=%p\n", this, requests);
}

::S3Status BucketTest::properties(const ::S3ResponseProperties * properties) {
	show(properties, Logger::DEBUG);
}

void BucketTest::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger & logger = Logger::instance();
	status = s3status;
	Logger::Level level = (isInaccessible() || isExistent() || isNonexistent()) ? Logger::DEBUG : Logger::ERROR;
	logger.log(level, "BucketTest@%p: status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	show(errorDetails, level);
	if (constraint[0] != '\0') {
		logger.debug("BucketTest@%p: constraint=\"%s\"\n", this, constraint);
	}
}

::S3Status BucketTest::responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData) {
	BucketTest * that = static_cast<BucketTest*>(callbackData);
	return that->properties(properties);
}

void BucketTest::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	BucketTest * that = static_cast<BucketTest*>(callbackData);
	that->complete(status, errorDetails);
}

}
}
}
}
