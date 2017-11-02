/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_
#define _H_COM_DIAG_HAYLOFT_S3_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

extern "C" {
#if 0
#include "libs3.h"
#else
#include <libs3.h>
#endif
}

namespace com {
namespace diag {
namespace hayloft {

/**
 * This type exists mostly because g++ 4.4.3 seems to have issues with
 * name space syntax inside template argument lists including C++ casts.
 */
typedef ::S3Status Status;

/**
 * This type exists mostly because g++ 4.4.3 seems to have issues with
 * name space syntax inside template argument lists including C++ casts.
 * Note that ::S3RequestContext is an opaque type in libs3.h.
 */
typedef ::S3RequestContext Handle;

}
}
}

#endif
