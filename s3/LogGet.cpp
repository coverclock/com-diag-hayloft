/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/LogGet.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/show.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void LogGet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	LogGet * that = static_cast<LogGet*>(callbackData);
	that->targetbucket[sizeof(that->targetbucket) - 1] = '\0';
	that->target = that->targetbucket;
	that->targetprefix[sizeof(that->targetprefix) - 1] = '\0';
	that->prefix = that->targetprefix;
	that->import(that->count, that->grants);
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("LogGet@%p: target=\"%s\"\n", that, that->target.c_str());
		logger.debug("LogGet@%p: prefix=\"%s\"\n", that, that->prefix.c_str());
		show(that->grants, that->count, Logger::DEBUG);
	}
	that->count = 0;
	delete [] that->grants;
	that->grants = 0;
	(*that->Log::handler.completeCallback)(status, errorDetails, callbackData);
}

LogGet::LogGet(const Bucket & bucket)
: Log(bucket)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

LogGet::LogGet(const Bucket & bucket, const Plex & plex)
: Log(bucket, plex)
, count(0)
, grants(0)
{
	initialize();
}

LogGet::~LogGet() {
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
	delete [] grants;
}

void LogGet::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	targetbucket[0] = '\0';
	targetprefix[0] = '\0';
	handler.propertiesCallback = Log::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void LogGet::execute() {
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
	count = 0;
	delete [] grants;
	grants = new ::S3AclGrant [S3_MAX_ACL_GRANT_COUNT];
	Logger::instance().debug("LogGet@%p: begin\n", this);
	Log::execute();
	S3_get_server_access_logging(
		&context,
		targetbucket,
		targetprefix,
		&count,
		grants,
		pending,
		&handler,
		this
	);
}

void LogGet::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
