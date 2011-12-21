/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/CriticalSection.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

static Mutex mutex;

static LifeCycle * instant = 0;

LifeCycle * LifeCycle::singleton = 0;

LifeCycle & LifeCycle::factory() {
    return (*(new LifeCycle));
}

LifeCycle & LifeCycle::instance(LifeCycle & that) {
	::com::diag::desperado::CriticalSection guard(mutex);
	singleton = &that;
    return *singleton;
}

LifeCycle & LifeCycle::instance() {
	::com::diag::desperado::CriticalSection guard(mutex);
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
	Logger::instance().debug("LifeCycle@%p: Action@%p: constructor\n", this, &action);
}

void LifeCycle::start(Action & action) {
	Logger::instance().debug("LifeCycle@%p: Action@%p: start\n", this, &action);
}

void LifeCycle::properties(Action & action, const ::S3ResponseProperties * properties) {
	Logger::instance().debug("LifeCycle@%p: Action@%p: properties\n", this, &action);
}

void LifeCycle::complete(Action & action, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("LifeCycle@%p: Action@%p: complete\n", this, &action);
}

void LifeCycle::destructor(Action & action) {
	Logger::instance().debug("LifeCycle@%p: Action@%p: destructor\n", this, &action);
}

}
}
}
}
