/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
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
