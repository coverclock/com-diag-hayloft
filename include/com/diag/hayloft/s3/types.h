/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_TYPES_
#define _H_COM_DIAG_HAYLOFT_S3_TYPES_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Date and time of day in seconds past the UNIX Epoch.
 */
typedef int64_t Epochalseconds;

/**
 * Size encoded in units of eight-bit bytes.
 */
typedef uint64_t Octets;

/**
 * Duration in milliseconds.
 */
typedef int64_t Milliseconds;

}
}
}
}

#endif
