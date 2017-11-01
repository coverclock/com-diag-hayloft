/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Protocol.h"

namespace com {
namespace diag {
namespace hayloft {

const ::S3Protocol Protocol::DEFAULT;

Protocol::Protocol(::S3Protocol scheme)
: protocol(scheme)
{}

}
}
}
