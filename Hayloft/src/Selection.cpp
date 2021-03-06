/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Selection.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

Selection::Selection(const char * namePrefix, const char * nextMarker, const char * nameDelimiter, int maxkeys)
: prefix(::com::diag::grandote::set(namePrefix))
, marker(::com::diag::grandote::set(nextMarker))
, delimiter(::com::diag::grandote::set(nameDelimiter))
, maximum(maxkeys)
{}

}
}
}
