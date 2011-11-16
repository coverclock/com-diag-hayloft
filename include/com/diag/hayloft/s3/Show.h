/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SHOW
#define _H_COM_DIAG_HAYLOFT_S3_SHOW

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Logger.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

extern void show(const ::S3ErrorDetails * details, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3ResponseProperties * response, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3BucketContext * context, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3PutProperties * properties, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3GetConditions * conditions, Logger::Level level = Logger::DEBUG);

}
}
}
}

#endif
