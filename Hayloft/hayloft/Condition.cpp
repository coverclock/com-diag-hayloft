/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Condition.h"
#include "com/diag/hayloft/Mutex.h"

namespace com {
namespace diag {
namespace hayloft {

Condition::Condition()
{
	::pthread_cond_init(&condition, 0);
}

Condition::~Condition() {
	::pthread_cond_broadcast(&condition);
	::pthread_yield();
	::pthread_cond_destroy(&condition);
}

int Condition::wait(Mutex & mutex) {
	return ::pthread_cond_wait(&condition, &mutex.mutex); // CANCELLATION POINT
}

int Condition::signal() {
	return ::pthread_cond_broadcast(&condition);
}

}
}
}