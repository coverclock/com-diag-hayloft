/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/CriticalSection.h"
#include "com/diag/hayloft/Mutex.h"

namespace com {
namespace diag {
namespace hayloft {

CriticalSection::CriticalSection(Mutex & rm)
: mutex(rm)
{
    this->mutex.begin(false);
}

CriticalSection::~CriticalSection() {
    this->mutex.end();
}

}
}
}
