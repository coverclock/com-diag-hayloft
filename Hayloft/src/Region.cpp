/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Region.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

Region::Region(const char * re)
: region(::com::diag::grandote::set(re, REGION_ENV(), DEFAULT()))
{}

}
}
}
