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

/**
 * Mutex implements mutual exclusion using a POSIX thread mutex.
 * This class is simply an alias for the Desperado Mutex. Note that by
 * default, the Desperado Mutex makes the calling thread uncancellable
 * while the Mutex is locked.
 */
typedef ::com::diag::desperado::Mutex Mutex;

}
}
}

#endif
