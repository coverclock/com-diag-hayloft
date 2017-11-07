/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Protocol.h"
#include "com/diag/hayloft/Debug.h"

namespace com {
namespace diag {
namespace hayloft {

const ::S3Protocol Protocol::DEFAULT = Debugging ? ::S3ProtocolHTTP : ::S3ProtocolHTTPS;

Protocol::Protocol(::S3Protocol scheme)
: protocol(scheme)
{}

}
}
}
