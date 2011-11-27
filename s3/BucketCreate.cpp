/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketCreate::BucketCreate(const char * bucketname, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
{
	initialize();
	execute();
}

BucketCreate::BucketCreate(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: Bucket(bucketname, multiplex, context, session)
{
	initialize();
}

BucketCreate::~BucketCreate() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketCreate::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
}

void BucketCreate::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("BucketCreate@%p: begin\n", this);
	::S3_create_bucket(
		protocol,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		access,
		(region.length() > 0) ? region.c_str() : 0,
		requests,
		&handler,
		this
	);
}

void BucketCreate::start() {
	if (state() != BUSY) {
		execute();
	}
}


}
}
}
}
