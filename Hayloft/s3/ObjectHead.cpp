/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectHead.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ObjectHead::ObjectHead(const char * keyname, const Bucket & bucket)
: Object(keyname, bucket)
{
	initialize();
	execute();
}

ObjectHead::ObjectHead(const char * keyname, const Bucket & bucket, const Multiplex & multiplex)
: Object(keyname, bucket, multiplex)
{
	initialize();
}

ObjectHead::~ObjectHead() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void ObjectHead::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
}

void ObjectHead::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectHead@%p: begin\n", this);
	::S3_head_object(
		&context,
		key.c_str(),
		requests,
		&handler,
		this
	);
}

void ObjectHead::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
