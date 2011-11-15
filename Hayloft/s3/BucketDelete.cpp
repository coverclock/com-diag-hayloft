/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketDelete::BucketDelete(const Session & session, const char * bucketname, const Context & context)
: Bucket(session, bucketname, context)
{
	initialize();
}

BucketDelete::BucketDelete(const Session & session, const char * bucketname, Queue & queue, const Context & context)
: Bucket(session, bucketname, queue, context)
{
	initialize();
}

BucketDelete::~BucketDelete() {
}

void BucketDelete::initialize() {
	Logger::instance().debug("BucketDelete@%p: begin\n", this);
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	::S3_delete_bucket(
		protocol,
		style,
		id.c_str(),
		secret.c_str(),
		hostname.c_str(),
		name.c_str(),
		requests,
		&handler,
		this
	);
}

void BucketDelete::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("BucketDelete@%p: end\n", this);
}

}
}
}
}
