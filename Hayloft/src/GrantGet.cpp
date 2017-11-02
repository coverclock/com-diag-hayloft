/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/GrantGet.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

void GrantGet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	GrantGet * that = static_cast<GrantGet*>(callbackData);
	that->ownerid[sizeof(that->ownerid) - 1] = '\0';
	that->owner = that->ownerid;
	that->ownerdisplayname[sizeof(that->ownerdisplayname) - 1] = '\0';
	that->display = that->ownerdisplayname;
	that->import(that->count, that->grants);
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("GrantGet@%p: owner=\"%s\"\n", that, that->owner.c_str());
		logger.debug("GrantGet@%p: display=\"%s\"\n", that, that->display.c_str());
		show(that->grants, that->count, ::com::diag::grandote::MaskableLogger::DEBUG);
	}
	that->count = 0;
	delete [] that->grants;
	that->grants = 0;
	(*that->Grant::handler.completeCallback)(status, errorDetails, callbackData);
}

GrantGet::GrantGet(const Bucket & bucket)
: Grant(bucket)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantGet::GrantGet(const Object & object)
: Grant(object)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantGet::GrantGet(const Bucket & bucket, const Plex & plex)
: Grant(bucket, plex)
, count(0)
, grants(0)
{
	initialize();
}

GrantGet::GrantGet(const Object & object, const Plex & plex)
: Grant(object, plex)
, count(0)
, grants(0)
{
	initialize();
}

GrantGet::~GrantGet() {
	delete [] grants;
}

void GrantGet::initialize() {
	state(static_cast<Status>(IDLE));
	ownerid[0] = '\0';
	ownerdisplayname[0] = '\0';
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void GrantGet::execute() {
	state(static_cast<Status>(BUSY));
	count = 0;
	delete [] grants;
	grants = new ::S3AclGrant [S3_MAX_ACL_GRANT_COUNT];
	::com::diag::grandote::MaskableLogger::instance().debug("GrantGet@%p: begin\n", this);
	Grant::execute();
	S3_get_acl(
		&context,
		keypointer,
		ownerid,
		ownerdisplayname,
		&count,
		grants,
		handle,
		&handler,
		this
	);
}

bool GrantGet::start() {
	if (Grant::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
