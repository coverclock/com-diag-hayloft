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
 * a Mutex by using the C++ "Resource Allocation is Initialization" idiom. The
 * critical section is protected by the associated Mutex while an object of
 * this type is in scope. This is equivalent to the Desperado CriticalSection
 * class but uses the Hayloft Mutex, which only differs by the Desperado
 * Mutex by making Condition a friend.
 *
 */
class CriticalSection {

protected:

    /**
     *  This is a reference to the mutex.
     */
    Mutex & mutex;

public:

    /**
     *  Constructor.
     *
     *  @param rm refers to a mutex object.
     */
    explicit CriticalSection(Mutex & rm);

    /**
     *  Destructor.
     */
    virtual ~CriticalSection();

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
