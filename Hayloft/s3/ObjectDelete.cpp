/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
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
}

void ObjectDelete::initialize() {
	state(static_cast<Status>(IDLE));
}

void ObjectDelete::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("ObjectDelete@%p: begin\n", this);
	Object::execute();
	::S3_delete_object(
		&context,
		key.c_str(),
		handle,
		&handler,
		this
	);
}

bool ObjectDelete::start(bool force) {
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
