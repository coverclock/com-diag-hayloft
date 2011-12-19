/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Thread.h"
#include "com/diag/desperado/errno.h"

namespace com {
namespace diag {
namespace hayloft {

void Thread::cleanup_mutex(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	pthread_mutex_unlock(&that->mutex);
}

// This is the one place we look at a variable that's not protected by our
// mutex. We do this because if we're not running at this point, then the
// object has been deleted out from under us and we no longer have a mutex
// or a condition variable.
void Thread::cleanup_thread(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	if (that->running) {
		pthread_mutex_lock(&that->mutex);
		pthread_cleanup_push(cleanup_mutex, that);
		that->running = false;
		pthread_cond_broadcast(&that->condition);
		pthread_cleanup_pop(!0);
	}
}

void * Thread::start_routine(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	pthread_cleanup_push(cleanup_thread, that);
	that->run();
	pthread_cleanup_pop(!0);
	return 0;
}

Thread::Thread()
: running(false)
{
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condition, 0);
}

// Unlike Java, there is no way to defer deleting this object if its thread is
// running. Wackiness ensues no whether the deleting thread is another thread
// or this thread itself. We try to do some stuff to minimize the damage, but
// we should consider treating this as a fatal error for the entire process.
Thread::~Thread() {
	bool self = false;
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		// Do nothing.
	} else if (!pthread_equal(pthread_self(), identity)) {
		pthread_cancel(identity);
		pthread_cond_wait(&condition, &mutex);
	} else {
		running = false;
		pthread_cond_broadcast(&condition);
		self = true;
	}
	pthread_cleanup_pop(!0);
	if (self) {
		pthread_yield();
	}
	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&mutex);
	if (self) {
		pthread_exit(0);
	}
}

int Thread::start() {
	int rc;
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		running = true;
		rc = pthread_create(&identity, 0, start_routine, this);
		if (rc != 0) { running = false; }
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

int Thread::cancel() {
	int rc;
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		rc = 0;
	} else if (!pthread_equal(pthread_self(), identity)) {
		rc = pthread_cancel(identity);
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

int Thread::join() {
	int rc;
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		rc = 0;
	} else if (!pthread_equal(pthread_self(), identity)) {
		rc = pthread_cond_wait(&condition, &mutex); // CANCELLATION POINT
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

void Thread::run() {
}

void Thread::quiescent() {
	pthread_testcancel();
}

void Thread::yield() {
	pthread_testcancel();
	pthread_yield();
	pthread_testcancel();
}

}
}
}
