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

BucketCreate::BucketCreate(const Session & se, const char * na, const Context & co)
: Bucket(se, na, co)
{
	create();
}

BucketCreate::BucketCreate(const Session & se, const char * na, Queue & qu, const Context & co)
: Bucket(se, na, qu, co)
{
	create();
}

BucketCreate::~BucketCreate() {
}

void BucketCreate::create() {
	Logger::instance().debug("BucketCreate@%p: begin\n", this);
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	::S3_create_bucket(
		context.getProtocol(),
		context.getId(),
		context.getSecret(),
		session.getHostName(),
		name.c_str(),
		context.getList(),
		(context.getLength() > 0) ? context.getConstraint() : 0,
		requests,
		&handler,
		this
	);
}

void BucketCreate::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("BucketCreate@%p: end\n", this);
	Bucket::complete(s3status, errorDetails);
}

}
}
}
}
