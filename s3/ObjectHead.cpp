/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
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

ObjectHead::ObjectHead(const char * keyname, const Bucket & bucket, const Plex & plex)
: Object(keyname, bucket, plex)
{
	initialize();
}

ObjectHead::ObjectHead(const Object & object)
: Object(object)
{
	initialize();
	execute();
}

ObjectHead::ObjectHead(const Object & object, const Plex & plex)
: Object(object, plex)
{
	initialize();
}

ObjectHead::~ObjectHead() {
	if (isBusy() && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void ObjectHead::initialize() {
	state(static_cast<Status>(IDLE));
}

void ObjectHead::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("ObjectHead@%p: begin\n", this);
	Object::execute();
	::S3_head_object(
		&context,
		key.c_str(),
		pending,
		&handler,
		this
	);
}

bool ObjectHead::start(bool force) {
	if ((!isBusy()) || force) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
}
