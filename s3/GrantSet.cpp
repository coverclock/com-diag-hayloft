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
	delete that->grants;
	that->count = 0;
	that->grants = 0;
	(*that->Grant::handler.completeCallback)(status, errorDetails, callbackData);
}

GrantSet::GrantSet(const Bucket & bucket)
: Grant(bucket)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Bucket & bucket, const Grant & grant)
: Grant(bucket, grant)
, count(0)
, grants(0)
{
	initialize();
	execute();
}

GrantSet::GrantSet(const Object & object)
: Grant(object)
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

GrantSet::GrantSet(const Bucket & bucket, const Multiplex & multiplex)
: Grant(bucket, multiplex)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::GrantSet(const Bucket & bucket, const Grant & grant, const Multiplex & multiplex)
: Grant(bucket, grant, multiplex)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Multiplex & multiplex)
: Grant(object, multiplex)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Grant & grant, const Multiplex & multiplex)
: Grant(object, grant, multiplex)
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
	delete grants;
	grants = new ::S3AclGrant [list.size()];
	count = 0;
	List::const_iterator here = list.begin();
	List::const_iterator there = list.end();
	while (here != there) {
		grants[count].granteeType = here->getGranteeType();
		grants[count].permission = here->getPermission();
		switch (here->getGranteeType()) {
		case ::S3GranteeTypeAmazonCustomerByEmail:
			std::strncpy(grants[count].grantee.amazonCustomerByEmail.emailAddress, here->getOwnerIdOrEmailAddress(), sizeof(grants[count].grantee.amazonCustomerByEmail.emailAddress));
			grants[count].grantee.amazonCustomerByEmail.emailAddress[sizeof(grants[count].grantee.amazonCustomerByEmail.emailAddress) - 1] = '\0';
			break;
		case ::S3GranteeTypeCanonicalUser:
			std::strncpy(grants[count].grantee.canonicalUser.id, here->getOwnerIdOrEmailAddress(), sizeof(grants[count].grantee.canonicalUser.id));
			grants[count].grantee.canonicalUser.id[sizeof(grants[count].grantee.canonicalUser.id) - 1] = '\0';
			std::strncpy(grants[count].grantee.canonicalUser.displayName, here->getOwnerDisplayName(), sizeof(grants[count].grantee.canonicalUser.displayName));
			grants[count].grantee.canonicalUser.displayName[sizeof(grants[count].grantee.canonicalUser.displayName) - 1] = '\0';
			break;
		default:
			break;
		}
		++here;
		++count;
	}
	show(grants, count);
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
