/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/GrantSet.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

void GrantSet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
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

GrantSet::GrantSet(const Bucket & bucket, const Plex & plex, const Grant & grant)
: Grant(bucket, plex, grant)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::GrantSet(const Object & object, const Plex & plex, const Grant & grant)
: Grant(object, plex, grant)
, count(0)
, grants(0)
{
	initialize();
}

GrantSet::~GrantSet() {
	delete [] grants;
}

void GrantSet::initialize() {
	state(static_cast<Status>(IDLE));
	handler.propertiesCallback = Grant::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("GrantSet@%p: owner=\"%s\"\n", this, owner.c_str());
		logger.debug("GrantSet@%p: display=\"%s\"\n", this, display.c_str());
	}
}

void GrantSet::execute() {
	state(static_cast<Status>(BUSY));
	delete [] grants;
	grants = generate(count);
	show(grants, count, ::com::diag::grandote::MaskableLogger::DEBUG);
	::com::diag::grandote::MaskableLogger::instance().debug("GrantSet@%p: begin\n", this);
	Grant::execute();
	S3_set_acl(
		&context,
		keypointer,
		owner.c_str(),
		display.c_str(),
		count,
		grants,
		handle,
		&handler,
		this
	);
}

bool GrantSet::start() {
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
