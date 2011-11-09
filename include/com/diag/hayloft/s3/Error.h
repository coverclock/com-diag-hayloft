/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ERROR
#define _H_COM_DIAG_HAYLOFT_S3_ERROR

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/generics.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

extern void show(const ::S3ErrorDetails * details);

}
}
}
}

#endif
