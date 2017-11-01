/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Selection.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Selection::Selection(const char * namePrefix, const char * nextMarker, const char * nameDelimiter, int maxkeys)
: prefix(set(namePrefix))
, marker(set(nextMarker))
, delimiter(set(nameDelimiter))
, maximum(maxkeys)
{}

}
}
}
}
