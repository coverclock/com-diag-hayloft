/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/GrantGet.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/show.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void GrantGet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	GrantGet * that = static_cast<GrantGet*>(callbackData);
	that->ownerid[sizeof(that->ownerid) - 1] = '\0';
	that->owner = that->ownerid;
	that->ownerdisplayname[sizeof(that->ownerdisplayname) - 1] = '\0';
	that->display = that->ownerdisplayname;
	that->import(that->count, that->grants);
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("GrantGet@%p: owner=\"%s\"\n", that, that->owner.c_str());
		logger.debug("GrantGet@%p: display=\"%s\"\n", that, that->display.c_str());
		show(that->grants, that->count, Logger::DEBUG);
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
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
	delete [] grants;
}

void GrantGet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void GrantGet::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	ownerid[0] = '\0';
	ownerdisplayname[0] = '\0';
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void GrantGet::execute() {
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
	count = 0;
	delete [] grants;
	grants = new ::S3AclGrant [S3_MAX_ACL_GRANT_COUNT];
	Logger::instance().debug("GrantGet@%p: begin\n", this);
	S3_get_acl(
		&context,
		keypointer,
		ownerid,
		ownerdisplayname,
		&count,
		grants,
		pending,
		&handler,
		this
	);
}

}
}
}
}
