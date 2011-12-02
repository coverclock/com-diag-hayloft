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
	(*that->Grant::handler.completeCallback)(status, errorDetails, callbackData);
}

GrantGet::GrantGet(const Bucket & bucket)
: Grant(bucket)
{
	initialize();
	execute();
}

GrantGet::GrantGet(const Object & object, const Bucket & bucket)
: Grant(object, bucket)
{
	initialize();
	execute();
}

GrantGet::GrantGet(const Bucket & bucket, const Multiplex & multiplex)
: Grant(bucket, multiplex)
{
	initialize();
}

GrantGet::GrantGet(const Object & object, const Bucket & bucket, const Multiplex & multiplex)
: Grant(object, bucket, multiplex)
{
	initialize();
}

GrantGet::~GrantGet() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void GrantGet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void GrantGet::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	grants = new ::S3AclGrant [COUNT];
	ownerid[0] = '\0';
	ownerdisplayname[0] = '\0';
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void GrantGet::execute() {
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
