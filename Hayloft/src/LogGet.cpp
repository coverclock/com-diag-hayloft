/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/LogGet.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

void LogGet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	LogGet * that = static_cast<LogGet*>(callbackData);
	that->targetbucket[sizeof(that->targetbucket) - 1] = '\0';
	that->target = that->targetbucket;
	that->targetprefix[sizeof(that->targetprefix) - 1] = '\0';
	that->prefix = that->targetprefix;
	that->import(that->count, that->grants);
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("LogGet@%p: target=\"%s\"\n", that, that->target.c_str());
		logger.debug("LogGet@%p: prefix=\"%s\"\n", that, that->prefix.c_str());
		show(that->grants, that->count, ::com::diag::grandote::MaskableLogger::DEBUG);
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
	::com::diag::grandote::MaskableLogger::instance().debug("LogGet@%p: begin\n", this);
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
