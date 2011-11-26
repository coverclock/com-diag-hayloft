/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketHead.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketHead::BucketHead(const char * bucketname, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
{
	initialize();
	execute();
}

BucketHead::BucketHead(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: Bucket(bucketname, multiplex, context, session)
{
	initialize();
}

BucketHead::~BucketHead() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketHead::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	constraint[0] = '\0';
}

void BucketHead::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("BucketHead@%p: begin\n", this);
	::S3_test_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		sizeof(constraint) - 1, constraint,
		requests,
		&handler,
		this
	);
}

void BucketHead::start() {
	if (state() != BUSY) {
		execute();
	}
}

void BucketHead::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Bucket::complete(status, errorDetails);
	constraint[sizeof(constraint) - 1] = '\0';
	region = constraint;
	Logger::instance().debug("BucketHead@%p: end\n", this);
}

}
}
}
}
