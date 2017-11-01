/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Access.h"

namespace com {
namespace diag {
namespace hayloft {

const ::S3CannedAcl Access::DEFAULT;

Access::Access(::S3CannedAcl cannedAcl)
: access(cannedAcl)
{}

}
}
}
