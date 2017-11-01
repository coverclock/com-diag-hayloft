/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/LogSet.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {

void LogSet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	LogSet * that = static_cast<LogSet*>(callbackData);
	delete [] that->grants;
	that->count = 0;
	that->grants = 0;
	(*that->Log::handler.completeCallback)(status, errorDetails, callbackData);
}


LogSet::LogSet(const Bucket & bucket, const Bucket & log, const char * keyprefix, const Grant & grant)
: Log(bucket, log, keyprefix, grant)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

LogSet::LogSet(const Bucket & bucket, const Plex & plex, const Bucket & log, const char * keyprefix, const Grant & grant)
: Log(bucket, plex, log, keyprefix, grant)
, count(0)
, grants(0)
{
	initialize();
}

LogSet::~LogSet() {
	delete [] grants;
}

void LogSet::initialize() {
	state(static_cast<Status>(IDLE));
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("LogSet@%p: target=\"%s\"\n", this, target.c_str());
		logger.debug("LogSet@%p: prefix=\"%s\"\n", this, prefix.c_str());
	}
}

void LogSet::execute() {
	state(static_cast<Status>(BUSY));
	delete [] grants;
	grants = generate(count);
	show(grants, count, Logger::DEBUG);
	Logger::instance().debug("LogSet@%p: begin\n", this);
	Log::execute();
	S3_set_server_access_logging(
		&context,
		target.c_str(),
		::com::diag::grandote::set(prefix),
		count,
		grants,
		handle,
		&handler,
		this
	);
}

bool LogSet::start() {
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
