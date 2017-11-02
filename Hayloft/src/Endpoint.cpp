/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Endpoint.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

Endpoint::Endpoint(const char * ep)
: endpoint(::com::diag::grandote::set(ep, ENDPOINT_ENV(), DEFAULT()))
{}

}
}
}
