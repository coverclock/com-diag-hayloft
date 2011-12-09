/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/LogSet.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void LogSet::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	LogSet * that = static_cast<LogSet*>(callbackData);
	delete [] that->grants;
	that->count = 0;
	that->grants = 0;
	(*that->Log::handler.completeCallback)(status, errorDetails, callbackData);
}


LogSet::LogSet(const Bucket & bucket, const Bucket & log, const Grant & grant, const char * keyprefix)
: Log(bucket, log, grant, keyprefix)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

LogSet::LogSet(const Bucket & bucket, const Multiplex & multiplex, const Bucket & log, const Grant & grant, const char * keyprefix)
: Log(bucket, multiplex, log, grant, keyprefix)
, count(0)
, grants(0)
{
	initialize();
}

LogSet::~LogSet() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
	delete [] grants;
}

void LogSet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void LogSet::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("LogSet@%p: target=\"%s\"\n", this, target.c_str());
		logger.debug("LogSet@%p: prefix=\"%s\"\n", this, prefix.c_str());
	}
}

void LogSet::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
	delete [] grants;
	grants = generate(count);
	show(grants, count, Logger::DEBUG);
	Logger::instance().debug("LogSet@%p: begin\n", this);
	S3_set_server_access_logging(
		&context,
		target.c_str(),
		set(prefix),
		count,
		grants,
		requests,
		&handler,
		this
	);
}

}
}
}
}
