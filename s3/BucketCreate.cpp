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

BucketCreate::BucketCreate(const char * bucketname, const Plex & plex, const Context & context, const Session & session)
: Bucket(bucketname, plex, context, session)
{
	initialize();
}

BucketCreate::BucketCreate(const Bucket & bucket)
: Bucket(bucket)
{
	initialize();
	execute();
}

BucketCreate::BucketCreate(const Bucket & bucket, const Plex & plex)
: Bucket(bucket, plex)
{
	initialize();
}

BucketCreate::~BucketCreate() {
	if (isBusy() && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void BucketCreate::initialize() {
	state(static_cast<Status>(IDLE));
}

void BucketCreate::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("BucketCreate@%p: begin\n", this);
	Bucket::execute();
	::S3_create_bucket(
		protocol,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		access,
		(region.length() > 0) ? region.c_str() : 0,
		pending,
		&handler,
		this
	);
}

bool BucketCreate::start(bool force) {
	if ((!isBusy()) || force) {
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
