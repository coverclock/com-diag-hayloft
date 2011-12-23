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

void * Thread::dontcare = 0;

pthread_key_t Thread::key;

Thread Thread::main(pthread_self());

int Thread::setup() {
	int rc = ::pthread_key_create(&key, 0);
	if (rc == 0) {
		rc = ::pthread_setspecific(key, &main);
	}
	return rc;
}

int Thread::handle = Thread::setup();

Thread & Thread::instance() {
	return *(static_cast<Thread *>(::pthread_getspecific(key)));
}

void Thread::yield() {
	::pthread_yield();
}

void Thread::exit() {
	::pthread_exit(reinterpret_cast<void*>(1));
}

::pthread_t Thread::self() {
	return ::pthread_self();
}

Thread::Thread()
: running(false)
, notifying(false)
, canceling(false)
, function(0)
, identity(0)
{
    initialize();
}

Thread::Thread(::pthread_t id)
: running(true)
, notifying(false)
, canceling(false)
, function(0)
, identity(id)
{
    initialize();
}

void Thread::cleanup_mutex(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	::pthread_mutex_unlock(&that->mutex);
}

// Unlike Java, there is no way to defer deleting this object if its thread is
// running. Wackiness ensues no whether the deleting thread is another thread
// or this thread itself. We try to do some stuff to minimize the damage, but
// we should consider treating this as a fatal error for the entire process.
Thread::~Thread() {
	bool self = false;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		// Do nothing.
	} else if (!::pthread_equal(pthread_self(), identity)) {
		::pthread_cancel(identity);
		::pthread_cond_wait(&condition, &mutex);
	} else {
		running = false;
		::pthread_cond_broadcast(&condition);
		self = true;
	}
	pthread_cleanup_pop(!0);
	if (self) {
		::pthread_yield();
	}
	::pthread_cond_destroy(&condition);
	::pthread_mutex_destroy(&mutex);
	if (self) {
		::pthread_exit(reinterpret_cast<void*>(-1));
	}
}

void Thread::initialize() {
	::pthread_mutex_init(&mutex, 0);
	::pthread_cond_init(&condition, 0);
}

void * Thread::empty_function(void *) {
	return 0;
}

void * Thread::run() {
	return (function != 0) ? (*function)(context) : 0;
}

void Thread::cleanup_thread(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	if (that->running) {
		::pthread_mutex_lock(&that->mutex);
		that->running = false;
		::pthread_cond_broadcast(&that->condition);
		::pthread_mutex_unlock(&that->mutex);
	}
}

void * Thread::start_routine(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	void * rc = reinterpret_cast<void *>(-1);
	int dontcare;
	::pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &dontcare);
	::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &dontcare);
	::pthread_setspecific(key, that);
	pthread_cleanup_push(cleanup_thread, that);
	rc = that->run();
	pthread_cleanup_pop(!0);
	return rc;
}

int Thread::start(Function & implementation, void * data) {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		running = true;
		notifying = false;
		function = &implementation;
		context = data;
		rc = ::pthread_create(&identity, 0, start_routine, this);
		if (rc != 0) { running = false; }
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

int Thread::notify() {
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	notifying = true;
	pthread_cleanup_pop(!0);
	return 0;
}

bool Thread::notified() {
	bool result;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	result = notifying;
	pthread_cleanup_pop(!0);
	return result;
}

int Thread::wait() {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		rc = 0;
	} else if (!::pthread_equal(pthread_self(), identity)) {
		rc = ::pthread_cond_wait(&condition, &mutex); // CANCELLATION POINT
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

/*******************************************************************************
 * The methods below are not your friends.
 * At best they should be unnecessary.
 * At worst they are dangerous.
 * They are provided for completeness.
 * Avoid using them.
 ******************************************************************************/

int Thread::join(void * & final) {
	return ::pthread_join(identity, &final);
}

void Thread::cancellable() {
	::pthread_testcancel();
}

int Thread::cancel() {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	if (!running) {
		rc = 0;
	} else if (!::pthread_equal(pthread_self(), identity)) {
		canceling = true;
		rc = ::pthread_cancel(identity);
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

bool Thread::cancelled() {
	bool result;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex, this);
	result = canceling;
	pthread_cleanup_pop(!0);
	return result;
}

}
}
}
