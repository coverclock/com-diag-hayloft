/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_TOSTRING_
#define _H_COM_DIAG_HAYLOFT_TOSTRING_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

class Container;
class Object;

/**
 * Convert an ::S3Status to its C string representation.
 *
 * @param status is an ::S3Status.
 * @return a C string representation.
 */
extern const char * tostring(Status status);

/**
 * Convert an ::S3Protocol to its C string representation.
 *
 * @param protocol is an ::S3Protocol.
 * @return a C string representation.
 */
extern const char * tostring(::S3Protocol protocol);

/**
 * Convert an ::S3UriStyle to its C string representation.
 *
 * @param style is an ::S3UriStyle.
 * @return a C string representation.
 */
extern const char * tostring(::S3UriStyle style);

/**
 * Convert an ::S3CannedAcl to its C string representation.
 *
 * @param access is an ::S3CannedAcl.
 * @return a C string representation.
 */
extern const char * tostring(::S3CannedAcl access);

/**
 * Convert an ::S3GranteeType to its C string representation.
 *
 * @param type is an ::S3GranteeType.
 * @return a C string representation.
 */
extern const char * tostring(::S3GranteeType type);

/**
 * Convert an ::S3Permission to its C string representation.
 *
 * @param permission is an ::S3Permission.
 * @return a C string representation.
 */
extern const char * tostring(::S3Permission permission);

/**
 * Convert a Container (Bucket) to its C string representation which is a URI.
 *
 * @param container refers to a Container.
 * @param uri refers to a string into which the representation is placed.
 * @return a C string representation.
 */
extern const char * tostring(const Container & container, std::string & uri);

/**
 * Convert an Object to its C string representation which is a URI.
 *
 * @param object refers to an Object.
 * @param uri refers to a string into which the representation is placed.
 * @return a C string representation.
 */
extern const char * tostring(const Object & object, std::string & uri);

}
}
}

#endif
