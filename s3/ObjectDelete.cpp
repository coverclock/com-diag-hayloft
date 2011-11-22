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
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ObjectDelete::ObjectDelete(const char * keyname, const Bucket & bucket)
: Object(keyname, bucket)
{
	initialize();
	execute();
}

ObjectDelete::ObjectDelete(const char * keyname, const Bucket & bucket, Multiplex & multiplex)
: Object(keyname, bucket, multiplex)
{
	initialize();
}

ObjectDelete::~ObjectDelete() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}

void ObjectDelete::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
}

void ObjectDelete::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
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
	Logger::instance().debug("ObjectDelete@%p: end\n", this);
}

}
}
}
}
