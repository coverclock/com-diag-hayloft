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
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void BucketHead::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	constraint[0] = '\0';
	handler.propertiesCallback = Bucket::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void BucketHead::execute() {
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("BucketHead@%p: begin\n", this);
	Bucket::execute();
	::S3_test_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		canonical.c_str(),
		sizeof(constraint) - 1, constraint,
		pending,
		&handler,
		this
	);
}

void BucketHead::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
