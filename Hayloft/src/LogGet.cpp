/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/LogGet.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/show.h"

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
	delete [] grants;
}

void LogGet::initialize() {
	state(static_cast<Status>(IDLE));
	targetbucket[0] = '\0';
	targetprefix[0] = '\0';
	handler.propertiesCallback = Log::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void LogGet::execute() {
	state(static_cast<Status>(BUSY));
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
		handle,
		&handler,
		this
	);
}

bool LogGet::start() {
	if (Log::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
}
