/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_MUTEX_
#define _H_COM_DIAG_HAYLOFT_MUTEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/Mutex.h"

namespace com {
namespace diag {
namespace hayloft {

class Condition;

/**
 * Mutex implements mutual exclusion using a POSIX thread mutex.
 * This class is mostly an alias for the Desperado Mutex class with
 * additional support for the Hayloft Condition class which must
 * access the POSIX thread mutex in the Mutex object. Like Desperado Mutex,
 * Hayloft Mutex allows recursive locking by the same thread of control.
 */
class Mutex : public ::com::diag::desperado::Mutex {

	friend class Condition;

};

}
}
}

#endif
