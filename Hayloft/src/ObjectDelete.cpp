/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

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
	::com::diag::grandote::MaskableLogger::instance().debug("ObjectDelete@%p: begin\n", this);
	Object::execute();
	::S3_delete_object(
		&context,
		key.c_str(),
		handle,
		&handler,
		this
	);
}

bool ObjectDelete::start() {
	if (Object::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
