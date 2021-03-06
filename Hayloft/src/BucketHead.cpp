/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/BucketHead.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

void BucketHead::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	BucketHead * that = static_cast<BucketHead*>(callbackData);
	that->constraint[sizeof(constraint) - 1] = '\0';
	that->region = that->constraint;
	(*that->Bucket::handler.completeCallback)(status, errorDetails, callbackData);
}

BucketHead::BucketHead(const char * bucketname, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
{
	initialize();
	execute();
}

BucketHead::BucketHead(const char * bucketname, const Plex & plex, const Context & context, const Session & session)
: Bucket(bucketname, plex, context, session)
{
	initialize();
}

BucketHead::BucketHead(const Bucket & bucket)
: Bucket(bucket)
{
	initialize();
	execute();
}

BucketHead::BucketHead(const Bucket & bucket, const Plex & plex)
: Bucket(bucket, plex)
{
	initialize();
}

BucketHead::~BucketHead() {
}

void BucketHead::initialize() {
	state(static_cast<Status>(IDLE));
	constraint[0] = '\0';
	handler.propertiesCallback = Bucket::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void BucketHead::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("BucketHead@%p: begin\n", this);
	Bucket::execute();
	::S3_test_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		sizeof(constraint) - 1, constraint,
		handle,
		&handler,
		this
	);
}

bool BucketHead::start() {
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
