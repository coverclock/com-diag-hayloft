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

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

GrantSet::GrantSet(const Bucket & bucket)
: Grant(bucket)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Bucket & bucket, const Grant & grant)
: Grant(bucket, grant)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Object & object)
: Grant(object)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Object & object, const Grant & grant)
: Grant(object, grant)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Bucket & bucket, const Multiplex & multiplex)
: Grant(bucket, multiplex)
{
	initialize();
}

GrantSet::GrantSet(const Bucket & bucket, const Grant & grant, const Multiplex & multiplex)
: Grant(bucket, grant, multiplex)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Multiplex & multiplex)
: Grant(object, multiplex)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Grant & grant, const Multiplex & multiplex)
: Grant(object, grant, multiplex)
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
}

void GrantSet::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(IDLE)?
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
