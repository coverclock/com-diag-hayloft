/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketDelete::BucketDelete(const char * bucketname, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
{
	initialize();
	execute();
}

BucketDelete::BucketDelete(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: Bucket(bucketname, multiplex, context, session)
{
	initialize();
}

BucketDelete::~BucketDelete() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketDelete::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
}

void BucketDelete::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("BucketDelete@%p: begin\n", this);
	::S3_delete_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		name.c_str(),
		requests,
		&handler,
		this
	);
}

void BucketDelete::start() {
	if (state() != BUSY) {
		execute();
	}
}

void BucketDelete::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Bucket::complete(status, errorDetails);
	Logger::instance().debug("BucketDelete@%p: end\n", this);
}

}
}
}
}
