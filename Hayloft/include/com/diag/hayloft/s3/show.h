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
struct S3AclGrant;

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;
class Object;
class ServiceManifest;
class BucketManifest;
class Grant;

/**
 * Display a libS3 S3ErrorDetails structure.
 *
 * @param details points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3ErrorDetails * details, Logger::Level level = Logger::DEBUG);

/**
 * Display a libS3 S3ResponseProperties structure.
 *
 * @param response points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3ResponseProperties * response, Logger::Level level = Logger::DEBUG);

/**
 * Display a libS3 S3BucketContext structure.
 *
 * @param context points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3BucketContext * context, Logger::Level level = Logger::DEBUG);

/**
 * Display a libS3 S3PutProperties structure.
 *
 * @param properties points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3PutProperties * properties, Logger::Level level = Logger::DEBUG);

/**
 * Display a libS3 S3GetConditions structure.
 *
 * @param conditions points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3GetConditions * conditions, Logger::Level level = Logger::DEBUG);

/**
 * Display an array of libS3 S3AclGrant structures.
 *
 * @param grant points to the structure.
 * @param count is the number of structures (if it is an array).
 * @param level specifies the logging level.
 */
extern void show(const ::S3AclGrant * grant, int count = 1, Logger::Level level = Logger::DEBUG);

/**
 * Display the metadata associated with a Hayloft Object.
 *
 * @param object refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Object & object, Logger::Level level = Logger::DEBUG);

/**
 * Display the metadata associated with a Hayloft Bucket.
 *
 * @param bucket refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Bucket & bucket, Logger::Level level = Logger::DEBUG);

/**
 * Display the metadata associated with a Hayloft ServiceManifest.
 *
 * @param manifest refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const ServiceManifest & manifest, Logger::Level level = Logger::DEBUG);

/**
 * Display the metadata associated with a Hayloft BucketManifest.
 *
 * @param manifest refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const BucketManifest & manifest, Logger::Level level = Logger::DEBUG);

/**
 * Display the metadata associated with a Hayloft Grant.
 *
 * @param grant refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Grant & grant, Logger::Level level = Logger::DEBUG);

}
}
}
}

#endif
