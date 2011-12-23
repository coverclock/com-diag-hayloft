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

/**
 * Thread implements a POSIX thread which can be associated with a thread of
 * control, that is, an independent context via which a processor may execute
 * instructions with other threads of control on the same processor
 * (serially via context switching) or on other processors (concurrently).
 */
class Thread {

public:

	/**
	 * This defines the prototype for a function that can be started in the
	 * new thread of control as an alternative to overriding the run method.
	 */
	typedef void * (Function)(void * data);

	/**
	 * Returns an reference to the Thread that represents the calling thread
	 * of control. Hayloft guarantees that all calls to this class method
	 * will return the same Thread object for the same calling thread of
	 * control.
	 *
	 * @return a reference to the Thread representing the calling thread.
	 */
	static Thread & instance();

	/**
	 * Cause the calling thread of control to yield the processor to another
	 * thread of control that is ready to run if such a thread exists.
	 */
	static void yield();

	/**
	 * Cause the calling thread of control to exit.
	 */
	static void exit();

	/**
	 * Return the POSIX thread identifier for the calling thread of control.
	 *
	 * @return the POSIX thread idenifier for the calling thread of control.
	 */
	static ::pthread_t self();

protected:

	static void * dontcare;

	static Thread main;

	static ::pthread_key_t key;

	static int handle;

	static int setup(void);

	static void cleanup_mutex(void * arg);

	static void cleanup_thread(void * arg);

	static void * start_routine(void * arg);

	static void * empty_function(void * context);

	bool running;

	bool notifying;

	bool canceling;

	Function * function;

	void * context;

	::pthread_t identity;

	::pthread_mutex_t mutex;

	::pthread_cond_t condition;

	explicit Thread(::pthread_t id);

	/**
	 * The derived class may override this method to provide an implementation
	 * for the thread of control to execute with this Thread is started. This
	 * is an alternative to providing a function.
	 */
	virtual void * run();

public:

	/**
	 * Constructor. A Thread object is created, but its thread of control is
	 * not automatically started.
	 */
	explicit Thread();

	/**
	 * Destructor. Deleting a Thread while its thread of control is running is
	 * really a bad idea. The destructor tries to minimize the damange, but
	 * chances are you are looking at an imminent application crash.
	 */
	virtual ~Thread();

	/**
	 * Start a new thread of control which will be associated with this
	 * Thread object. A new thread of control can only be started when there
	 * is not one already running.
	 *
	 * @param implementation refers to a function that is executed in the new
	 *        thread of control as an alternative to overriding the run method.
	 * @param data is passed to the function in the new thread of control.
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int start(Function & implementation = empty_function, void * data = 0);

	/**
	 * Wait for the thread of control associated with this Thread to complete.
	 * Multiple threads of control can wait on the same Thread. It is an error
	 * for a thread of control to wait on itself.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int wait();

	/**
	 * Notify the Thread. This merely sets the a flag in this Thread which can
	 * be interrogated by the notified method. Once a Thread is notified, it
	 * remains notified until it completes and is re-started.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int notify();

	/**
	 * Return the notification state of this Thread.
	 *
	 * @return true if this Thread has been notified, false otherwise.
	 */
	virtual bool notified();

	/**
	 * Get the POSIX thread identifier for the thread of control associated with
	 * this Thread. This value has no meaning if the thread of control has
	 * never been started, and the returned value is undefined.
	 *
	 * @return the POSIX thread identifier associated with this Thread.
	 */
	::pthread_t getIdentity() { return identity; }

private:

	void initialize();

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

public:

	/***************************************************************************
	 * You should endeavor not to use methods in this section. They are not your
	 * friends. Wackiness may ensue. They are provided for completeness. You
	 * have been warned.
	 **************************************************************************/

	/**
	 * Provide an explicit POSIX thread cancellation point for the calling
	 * thread of control. Using cancel semantics is a really bad idea for lots
	 * of reasons, but this method is provided for completeness.
	 */
	static void cancellable();

	/**
	 * Cancel the thread of control associated with this Thread. Once a Thread
	 * is canceled, it remains canceled until it completes and is re-started.
	 * Using cancel semantics is a really bad idea for lots of reasons, but
	 * this method is provided for completeness.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int cancel();

	/**
	 * Return true if this Thread has been cancelled. Using cancel
	 * semantics is a really bad idea for lots of reasons, but this method
	 * is provided for completeness.
	 *
	 * @return true if this Thread has been cancelled, false otherwise.
	 */
	virtual bool cancelled();

	/**
	 * Block the calling thread of control until the thread of control
	 * associated with this Thread has completed or been canceled. POSIX
	 * allows only a single thread of control to join with another thread.
	 * Calling join on a Thread that has never been started is likely to
	 * crash the application (and there is no reliable way to fix this).
	 * Some POSIX implementations encourage this call to clean up system
	 * resources associated with a thread. For others it is merely a
	 * conveniences. This method is provided for completeness.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int join(void * & final = dontcare);

};

}
}
}

#endif
