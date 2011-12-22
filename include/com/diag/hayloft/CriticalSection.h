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

namespace com {
namespace diag {
namespace hayloft {

class Mutex;

/**
 * CriticalSection locks a Mutex in its constructor and unlocks it in its
 * destructor. This allows you to implement a critical section protected by
 * a Mutex using the C++ "Resource Allocation is Initialization" idiom.
 * Unlike the Desperado CriticalSection class, this class does not make the
 * calling thread uncancellable.
 */
class CriticalSection {

public:

    /**
     *  Constructor.
     *
     *  @param mutexr  refers to a mutex object.
     */
    explicit CriticalSection(Mutex & rm);

    /**
     *  Destructor.
     */
    virtual ~CriticalSection();

protected:

    /**
     *  This is a reference to the mutex.
     */
    Mutex & mutex;

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    CriticalSection(const CriticalSection & that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    CriticalSection& operator=(const CriticalSection & that);

};

}
}
}

#endif
