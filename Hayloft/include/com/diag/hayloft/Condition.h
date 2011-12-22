/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_CONDITION_
#define _H_COM_DIAG_HAYLOFT_CONDITION_

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

class Mutex;

class Condition {

protected:

	::pthread_cond_t condition;

public:

	explicit Condition();

	virtual ~Condition();

	virtual int wait(Mutex & mutex);

	virtual int signal();

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Condition(const Condition& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Condition& operator=(const Condition& that);

};

}
}
}

#endif
