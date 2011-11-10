/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/CannedAccessControlList.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const ::S3CannedAcl CannedAccessControlList::DEFAULT;

CannedAccessControlList::CannedAccessControlList(::S3CannedAcl acl)
: list(acl)
{
	Logger::instance().debug("CannedAccessControlList@%p: list=%d\n", this, list);
}

}
}
}
}
