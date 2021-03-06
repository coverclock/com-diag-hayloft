/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/LifeCycle.h"
#include "com/diag/hayloft/Action.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/Mutex.h"
#include "com/diag/grandote/CriticalSection.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

static ::com::diag::grandote::Mutex mutex;

static LifeCycle * instant = 0;

LifeCycle * LifeCycle::singleton = 0;

LifeCycle & LifeCycle::factory() {
    return (*(new LifeCycle));
}

LifeCycle & LifeCycle::instance(LifeCycle & that) {
	::com::diag::grandote::CriticalSection guard(mutex);
	singleton = &that;
    return *singleton;
}

LifeCycle & LifeCycle::instance() {
	::com::diag::grandote::CriticalSection guard(mutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}

LifeCycle::LifeCycle()
{
}

LifeCycle::~LifeCycle() {
}

void LifeCycle::constructor(Action & action) {
	::com::diag::grandote::MaskableLogger::instance().debug("LifeCycle@%p: Action@%p: constructor\n", this, &action);
}

void LifeCycle::start(Action & action) {
	::com::diag::grandote::MaskableLogger::instance().debug("LifeCycle@%p: Action@%p: start\n", this, &action);
}

Status LifeCycle::properties(Action & action, const ::S3ResponseProperties * responseProperties) {
	::com::diag::grandote::MaskableLogger::instance().debug("LifeCycle@%p: Action@%p: properties\n", this, &action);
	return action.properties(responseProperties);
}

void LifeCycle::complete(Action & action, Status ultimate, const ::S3ErrorDetails * errorDetails) {
	::com::diag::grandote::MaskableLogger::instance().debug("LifeCycle@%p: Action@%p: complete\n", this, &action);
	action.signal(ultimate); // All polling or waiting Threads can see this.
	action.complete(ultimate, errorDetails);
}

void LifeCycle::destructor(Action & action) {
	::com::diag::grandote::MaskableLogger::instance().debug("LifeCycle@%p: Action@%p: destructor\n", this, &action);
}

}
}
}
