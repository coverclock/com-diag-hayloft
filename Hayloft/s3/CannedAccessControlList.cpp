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

CannedAccessControlList::CannedAccessControlList(::S3CannedAcl acl)
: canned(acl)
{
	Logger::instance().debug("CannedAccessControlList@%p: cannedacl=%d\n", this, canned);
}

}
}
}
}
