/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/BucketDelete.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

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
}

void BucketDelete::initialize() {
	state(static_cast<Status>(IDLE));
}

void BucketDelete::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("BucketDelete@%p: begin\n", this);
	Bucket::execute();
	::S3_delete_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		handle,
		&handler,
		this
	);
}

bool BucketDelete::start() {
	if (Bucket::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
