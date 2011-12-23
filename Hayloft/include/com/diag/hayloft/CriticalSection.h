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
 * a Mutex by using the C++ "Resource Allocation is Initialization" idiom. The
 * critical section is protected by the associated Mutex while an object of
 * this type is in scope. This class is an alias for the Desperado
 * CriticalSection class. Like Desperado CriticalSection, the default
 * behavior for Hayloft CriticalSection is to disable cancellation of the
 * constructing thread of control while an object of this type is in scope.
 *
 * I've had mixed experiences leaving cancellation enabled in the Hayloft unit
 * test suite. Generally, using thread cancellation is a bad idea, for lots
 * of reasons. Historically, best practices in using threading packages in
 * systems ranging from GNU/Linux to vxWorks recommend against it. Typically
 * this is because canceling a thread may subvert or bypass the normal
 * mechanisms that guarantee its acquired resources are released. My expectation
 * is that the C++ run time system should guarantee that the destructors for
 * stack objects would be executed. But this may be dependent on the specific
 * C++ implementation. I cannot recommend it as a general practice, but there
 * may be situations where it is necessary.
 */
typedef ::com::diag::desperado::CriticalSection CriticalSection;

}
}
}

#endif
