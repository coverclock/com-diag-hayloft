/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Conditions.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Conditions::Conditions(Epochalseconds ifModifiedSince, Epochalseconds ifNotModifiedSince, const char * ifMatchETag, const char * ifNotMatchETag)
: since(ifModifiedSince)
, notsince(ifNotModifiedSince)
, match(set(ifMatchETag, 0, ""))
, notmatch(set(ifNotMatchETag, 0, ""))
{}

}
}
}
}
