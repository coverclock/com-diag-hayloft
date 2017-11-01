/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/ObjectHead.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

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
}

void ObjectHead::initialize() {
	state(static_cast<Status>(IDLE));
}

void ObjectHead::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("ObjectHead@%p: begin\n", this);
	Object::execute();
	::S3_head_object(
		&context,
		key.c_str(),
		handle,
		&handler,
		this
	);
}

bool ObjectHead::start() {
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
