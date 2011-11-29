/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SHOW_
#define _H_COM_DIAG_HAYLOFT_S3_SHOW_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Logger.h"

struct S3ErrorDetails;
struct S3ResponseProperties;
struct S3BucketContext;
struct S3PutProperties;
struct S3GetConditions;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Object;

extern void show(const ::S3ErrorDetails * details, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3ResponseProperties * response, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3BucketContext * context, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3PutProperties * properties, Logger::Level level = Logger::DEBUG);

extern void show(const ::S3GetConditions * conditions, Logger::Level level = Logger::DEBUG);

extern void show(const Object & object, Logger::Level level = Logger::DEBUG);

}
}
}
}

#endif
