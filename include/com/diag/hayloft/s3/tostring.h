/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_TOSTRING_
#define _H_COM_DIAG_HAYLOFT_S3_TOSTRING_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Container;
class Object;

extern const char * tostring(::S3Status status);

extern const char * tostring(::S3Protocol protocol);

extern const char * tostring(::S3UriStyle style);

extern const char * tostring(::S3CannedAcl access);

extern const char * tostring(const Container & container, std::string & uri);

extern const char * tostring(const Object & object, std::string & uri);

}
}
}
}

#endif
