/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/GrantSet.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void GrantSet::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	GrantSet * that = static_cast<GrantSet*>(callbackData);
	delete [] that->grants;
	that->count = 0;
	that->grants = 0;
	(*that->Grant::handler.completeCallback)(status, errorDetails, callbackData);
}

GrantSet::GrantSet(const Bucket & bucket, const Grant & grant)
: Grant(bucket, grant)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Object & object, const Grant & grant)
: Grant(object, grant)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Bucket & bucket, const Multiplex & multiplex, const Grant & grant)
: Grant(bucket, multiplex, grant)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Multiplex & multiplex, const Grant & grant)
: Grant(object, multiplex, grant)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::~GrantSet() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void GrantSet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void GrantSet::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void GrantSet::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
	delete [] grants;
	grants = generate(count);
	show(grants, count, Logger::DEBUG);
	Logger::instance().debug("GrantSet@%p: begin\n", this);
	S3_set_acl(
		&context,
		keypointer,
		owner.c_str(),
		display.c_str(),
		count,
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
