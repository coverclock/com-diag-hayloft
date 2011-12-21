/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_CRITICALSECTION_
#define _H_COM_DIAG_HAYLOFT_CRITICALSECTION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/CriticalSection.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * CriticalSection locks a Mutex in its constructor and unlocks it in its
 * destructor. This allows you to implement a critical section protected by
 * a Mutex using the C++ "Resource Allocation is Initialization" idiom.
 * This class is simply an alias for the Desperado CriticalSection. Note
 * that by default, the Desperado Critical Section does not make the calling
 * Thread uncancellable, depending instead on the destructor to unlock the
 * Mutex.
 */
typedef ::com::diag::desperado::CriticalSection CriticalSection;

}
}
}

#endif
