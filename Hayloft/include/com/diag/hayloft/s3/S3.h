/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_S3_
#define _H_COM_DIAG_HAYLOFT_S3_S3_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

extern "C" {
#include "libs3.h"
}

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * This type exists mostly because g++ 4.4.3 seems to have issues with
 * name space syntax inside template argument lists.
 */
typedef ::S3Status Status;

/**
 * This type exists mostly because g++ 4.4.3 seems to have issues with
 * name space syntax inside template argument lists.
 */
typedef ::S3RequestContext Pending;

}
}
}
}

#endif
