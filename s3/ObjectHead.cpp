/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectHead.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ObjectHead::ObjectHead(const Bucket & bucket, const char * keyname)
: Object(bucket, keyname)
{
	initialize();
	begin();
}

ObjectHead::~ObjectHead() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void ObjectHead::initialize() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
}

void ObjectHead::begin() {
	Logger::instance().debug("ObjectHead@%p: begin\n", this);
	::S3_head_object(
		&context,
		key.c_str(),
		requests,
		&handler,
		this
	);
}

void ObjectHead::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("ObjectHead@%p: complete\n", this);
}

}
}
}
}
