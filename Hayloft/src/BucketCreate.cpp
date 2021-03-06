/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

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
}

void BucketCreate::initialize() {
	state(static_cast<Status>(IDLE));
}

void BucketCreate::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("BucketCreate@%p: begin\n", this);
	Bucket::execute();
	::S3_create_bucket(
		protocol,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		access,
		(region.length() > 0) ? region.c_str() : 0,
		handle,
		&handler,
		this
	);
}

bool BucketCreate::start() {
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
