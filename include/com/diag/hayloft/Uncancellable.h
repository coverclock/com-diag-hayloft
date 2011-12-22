/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_UNCANCELLABLE_
#define _H_COM_DIAG_HAYLOFT_UNCANCELLABLE_

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

/**
 * Uncancellable cases the current thread to become uncancellable while this
 * object is in scope.
 */
class Uncancellable {

public:

    /**
     *  Constructor.
     */
    explicit Uncancellable();

    /**
     *  Destructor.
     */
    virtual ~Uncancellable();

protected:

    int state;

};

}
}
}

#endif
