/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_SHOW_
#define _H_COM_DIAG_HAYLOFT_SHOW_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/grandote/MaskableLogger.h"

struct S3ErrorDetails;
struct S3ResponseProperties;
struct S3BucketContext;
struct S3PutProperties;
struct S3GetConditions;
struct S3AclGrant;

namespace com {
namespace diag {
namespace hayloft {

class Bucket;
class Object;
class ServiceManifest;
class BucketManifest;
class Grant;
class Log;

static const ::com::diag::grandote::MaskableLogger::Level SHOW_LOG_LEVEL = ::com::diag::grandote::MaskableLogger::INFORMATION;

/**
 * Display a libS3 S3ErrorDetails structure.
 *
 * @param details points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3ErrorDetails * details, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display a libS3 S3ResponseProperties structure.
 *
 * @param response points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3ResponseProperties * response, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display a libS3 S3BucketContext structure.
 *
 * @param context points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3BucketContext * context, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display a libS3 S3PutProperties structure.
 *
 * @param properties points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3PutProperties * properties, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display a libS3 S3GetConditions structure.
 *
 * @param conditions points to the structure.
 * @param level specifies the logging level.
 */
extern void show(const ::S3GetConditions * conditions, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display an array of libS3 S3AclGrant structures.
 *
 * @param grant points to the structure.
 * @param count is the number of structures (if it is an array).
 * @param level specifies the logging level.
 */
extern void show(const ::S3AclGrant * grant, int count = 1, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft Object.
 *
 * @param object refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Object & object, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft Bucket.
 *
 * @param bucket refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Bucket & bucket, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft ServiceManifest.
 *
 * @param manifest refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const ServiceManifest & manifest, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft BucketManifest.
 *
 * @param manifest refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const BucketManifest & manifest, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft Grant.
 *
 * @param grant refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Grant & grant, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

/**
 * Display the metadata associated with a Hayloft Log.
 *
 * @param log refers to the object.
 * @param level specifies the logging level.
 */
extern void show(const Log & log, ::com::diag::grandote::MaskableLogger::Level level = SHOW_LOG_LEVEL);

}
}
}

#endif
