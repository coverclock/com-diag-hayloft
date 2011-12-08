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

void GrantGet::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	GrantGet * that = static_cast<GrantGet*>(callbackData);
	that->ownerid[sizeof(that->ownerid) - 1] = '\0';
	that->owner = that->ownerid;
	that->ownerdisplayname[sizeof(that->ownerdisplayname) - 1] = '\0';
	that->display = that->ownerdisplayname;
	that->import(that->count, that->grants);
	show(that->grants, that->count, Logger::DEBUG);
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

GrantGet::GrantGet(const Bucket & bucket, const Multiplex & multiplex)
: Grant(bucket, multiplex)
, count(0)
, grants(0)
{
	initialize();
}

GrantGet::GrantGet(const Object & object, const Multiplex & multiplex)
: Grant(object, multiplex)
, count(0)
, grants(0)
{
	initialize();
}

GrantGet::~GrantGet() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
	delete [] grants;
}

void GrantGet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void GrantGet::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	ownerid[0] = '\0';
	ownerdisplayname[0] = '\0';
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void GrantGet::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
	count = 0;
	delete [] grants;
	grants = new ::S3AclGrant [COUNT];
	Logger::instance().debug("GrantGet@%p: begin\n", this);
	S3_get_acl(
		&context,
		keypointer,
		ownerid,
		ownerdisplayname,
		&count,
		grants,
		requests,
		&handler,
		this
	);
}

}
}
}
}
