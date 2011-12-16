/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Reaction.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Reaction::Reaction()
{
}

Reaction::~Reaction() {
}

Status Reaction::properties(Action & action, const ::S3ResponseProperties * properties) {
	Logger::instance().debug("Action@%p: properties@%p\n", &action, this);
	return ::S3StatusOK;
}

void Reaction::complete(Action & action, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("Action@%p: complete@%p\n", &action, this);
	return;
}

void Reaction::destructor(Action & action) {
	Logger::instance().debug("Action@%p: destructor@%p\n", &action, this);
}

}
}
}
}
