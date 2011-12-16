/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/Logger.h"

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

ObjectDelete::ObjectDelete(const char * keyname, const Bucket & bucket, const Plex & plex)
: Object(keyname, bucket, plex)
{
	initialize();
}

ObjectDelete::ObjectDelete(const Object & object)
: Object(object)
{
	initialize();
	execute();
}

ObjectDelete::ObjectDelete(const Object & object, const Plex & plex)
: Object(object, plex)
{
	initialize();
}

ObjectDelete::~ObjectDelete() {
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void ObjectDelete::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
}

void ObjectDelete::execute() {
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectDelete@%p: begin\n", this);
	::S3_delete_object(
		&context,
		key.c_str(),
		pending,
		&handler,
		this
	);
}

void ObjectDelete::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
