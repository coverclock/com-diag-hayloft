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

BucketDelete::BucketDelete(const char * bucketname, const Plex & plex, const Context & context, const Session & session)
: Bucket(bucketname, plex, context, session)
{
	initialize();
}

BucketDelete::BucketDelete(const Bucket & bucket)
: Bucket(bucket)
{
	initialize();
	execute();
}

BucketDelete::BucketDelete(const Bucket & bucket, const Plex & plex)
: Bucket(bucket, plex)
{
	initialize();
}

BucketDelete::~BucketDelete() {
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void BucketDelete::initialize() {
	state(static_cast<Status>(IDLE));
}

void BucketDelete::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("BucketDelete@%p: begin\n", this);
	Bucket::execute();
	::S3_delete_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		pending,
		&handler,
		this
	);
}

void BucketDelete::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
