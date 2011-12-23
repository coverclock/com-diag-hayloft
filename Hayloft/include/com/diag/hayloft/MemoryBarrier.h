/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_MEMORYBARRIER_
#define _H_COM_DIAG_HAYLOFT_MEMORYBARRIER_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/barrier.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * MemoryBarrier performs a full fence (both acquire and release semantics)
 * memory barrier, if such a thing is available on the target and platform,
 * in its constructor and destructor. This allows you to implement memory
 * synchronization by using the C++ "Resource Allocation is Initialization"
 * idiom. The memory fence is executed when an object of this type goes in and
 * out of scope.
 *
 * This is similar to the Desperado MemoryBarrier class.
 */
class MemoryBarrier {

public:

    /**
     *  Constructor.
     */
    MemoryBarrier() { com_diag_hayloft_memory_barrier(); }

    /**
     *  Destructor.
     */
    ~MemoryBarrier() { com_diag_hayloft_memory_barrier(); }

};

}
}
}

#endif
