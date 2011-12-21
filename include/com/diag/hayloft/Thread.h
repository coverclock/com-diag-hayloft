/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_THREAD_
#define _H_COM_DIAG_HAYLOFT_THREAD_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <pthread.h>

namespace com {
namespace diag {
namespace hayloft {

class Thread {

public:

	static pthread_t self();

	static void cancellable();

	static void yield();

	static void exit();

private:

	static void cleanup_mutex(void * arg);

	static void cleanup_thread(void * arg);

	static void * start_routine(void * arg);

	bool running;

	bool canceling;

	pthread_t identity;

    pthread_mutex_t mutex;

    pthread_cond_t condition;

protected:

	virtual void run();

public:

	explicit Thread();

	virtual ~Thread();

	virtual int start();

	virtual int cancel();

	virtual int join();

	virtual bool cancelled();

	pthread_t getIdentity() { return identity; }

protected:

	explicit Thread(pthread_t id);

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Thread(const Thread& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Thread& operator=(const Thread& that);

};

}
}
}

#endif
