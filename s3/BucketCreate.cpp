/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketCreate::BucketCreate(const Session & session, const char * bucketname, const Context & context)
: Bucket(session, bucketname, context)
{
	initialize();
	begin();
}

BucketCreate::BucketCreate(const Session & session, const char * bucketname, Multiplex & multiplex, const Context & context)
: Bucket(session, bucketname, multiplex, context)
{
	initialize();
	begin();
}

BucketCreate::~BucketCreate() {
}

void BucketCreate::initialize() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
}

void BucketCreate::begin() {
	Logger::instance().debug("BucketCreate@%p: begin\n", this);
	::S3_create_bucket(
		protocol,
		id.c_str(),
		secret.c_str(),
		hostname.c_str(),
		name.c_str(),
		access,
		(region.length() > 0) ? region.c_str() : 0,
		requests,
		&handler,
		this
	);
}

void BucketCreate::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("BucketCreate@%p: complete\n", this);
}

}
}
}
}
