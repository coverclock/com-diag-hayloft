/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ObjectDelete::ObjectDelete(const Bucket & bucket, const char * keyname)
: Object(bucket, keyname)
{
	initialize();
	begin();
}

ObjectDelete::~ObjectDelete() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void ObjectDelete::initialize() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
}

void ObjectDelete::begin() {
	Logger::instance().debug("ObjectDelete@%p: begin\n", this);
	::S3_delete_object(
		&context,
		key.c_str(),
		requests,
		&handler,
		this
	);
}

void ObjectDelete::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("ObjectDelete@%p: complete\n", this);
}

}
}
}
}
