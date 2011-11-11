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

BucketDelete::BucketDelete(const Session & se, const char * na, const Context & co)
: Bucket(se, na, co)
{
	remove();
}

BucketDelete::BucketDelete(const Session & se, const char * na, Queue & qu, const Context & co)
: Bucket(se, na, co)
{
	remove();
}

BucketDelete::~BucketDelete() {
}

void BucketDelete::remove() {
	Logger::instance().debug("BucketDelete@%p: begin\n", this);
	::S3_delete_bucket(
		context.getProtocol(),
		context.getStyle(),
		context.getId(),
		context.getSecret(),
		session.getHostName(),
		name.c_str(),
		requests,
		&handler,
		this
	);
}

void BucketDelete::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("BucketDelete@%p: end\n", this);
	Bucket::complete(s3status, errorDetails);
}

}
}
}
}
