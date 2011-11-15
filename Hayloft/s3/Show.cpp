/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/s3/Credentials.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void show(const ::S3ResponseProperties * properties, Logger::Level level) {
	if (properties != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (properties->requestId != 0) { logger.log(level, "S3ResponseProperties@%p: requestId=\"%s\"\n", properties, properties->requestId); }
			if (properties->requestId2 != 0) { logger.log(level, "S3ResponseProperties@%p: requestId2=\"%s\"\n", properties, properties->requestId2); }
			if (properties->contentType != 0) { logger.log(level, "S3ResponseProperties@%p: contentType=\"%s\"\n", properties, properties->contentType); }
			if (properties->contentLength != 0) { logger.log(level, "S3ResponseProperties@%p: contentLength=%llu\n", properties, properties->contentLength); }
			if (properties->server != 0) { logger.log(level, "S3ResponseProperties@%p: server=\"%s\"\n", properties, properties->server); }
			if (properties->eTag != 0) { logger.log(level, "S3ResponseProperties@%p: eTag=\"%s\"\n", properties, properties->eTag); }
			if (properties->lastModified != 0) { logger.log(level, "S3ResponseProperties@%p: lastModified=%lld\n", properties, properties->lastModified); }
			if (properties->metaData != 0) {
				for (int ii = 0; ii < properties->metaDataCount; ++ii) {
					if ((properties->metaData[ii].name != 0) || (properties->metaData[ii].value != 0)) {
						logger.log(level, "S3ResponseProperties@%p: \"%s\"=\"%s\"\n", properties,
								(properties->metaData[ii].name != 0) ? properties->metaData[ii].name : "",
								(properties->metaData[ii].value != 0) ? properties->metaData[ii].value : "");
					}
				}
			}
		}
	}
}

void show(const ::S3ErrorDetails * details, Logger::Level level) {
	if (details != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (details->message != 0) { logger.log(level, "S3ErrorDetails@%p: message=\"%s\"\n", details, details->message); }
			if (details->resource != 0) { logger.log(level, "S3ErrorDetails@%p: resource=\"%s\"\n", details, details->resource); }
			if (details->furtherDetails != 0) { logger.log(level, "S3ErrorDetails@%p: further=\"%s\"\n", details, details->furtherDetails); }
			if (details->extraDetails != 0) {
				for (int ii = 0; ii < details->extraDetailsCount; ++ii) {
					if ((details->extraDetails[ii].name != 0) || (details->extraDetails[ii].value != 0)) {
						logger.log(level, "S3ErrorDetails@%p: \"%s\"=\"%s\"\n", details,
								(details->extraDetails[ii].name != 0) ? details->extraDetails[ii].name : "",
								(details->extraDetails[ii].value != 0) ? details->extraDetails[ii].value : "");
					}
				}
			}
		}
	}
}


void show(const ::S3BucketContext * context, Logger::Level level) {
	if (context != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (context->hostName != 0) { logger.log(level, "S3BucketContext@%p: hostName=\"%s\"\n", context, context->hostName); }
			if (context->bucketName != 0) { logger.log(level, "S3BucketContext@%p: bucketName=\"%s\"\n", context, context->bucketName); }
			logger.log(level, "S3BucketContext@%p: protocol=\"%s\"\n", context, context->protocol);
			logger.log(level, "S3BucketContext@%p: uriStyle=\"%s\"\n", context, context->uriStyle);
			if (context->accessKeyId != 0) { logger.log(level, "S3BucketContext@%p: accessKeyId=\"%s\"\n", context, Credentials::obfuscate(context->accessKeyId)); }
			if (context->secretAccessKey != 0) { logger.log(level, "S3BucketContext@%p: secretAccessKey=\"%s\"\n", context, Credentials::obfuscate(context->secretAccessKey)); }
		}
	}
}


void show(const ::S3PutProperties * properties, Logger::Level level) {
	if (properties != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (properties->contentType != 0) { logger.log(level, "S3PutProperties@%p: contentType=\"%s\"\n", properties, properties->contentType); }
			if (properties->md5 != 0) { logger.log(level, "S3PutProperties@%p: md5=\"%s\"\n", properties, properties->md5); }
			if (properties->cacheControl != 0) { logger.log(level, "S3PutProperties@%p: cacheControl=\"%s\"\n", properties,  properties->cacheControl); }
			if (properties->contentDispositionFilename != 0) { logger.log(level, "S3PutProperties@%p: contentDispositionFilename=\"%s\"\n", properties, properties->contentDispositionFilename); }
			if (properties->contentEncoding != 0) { logger.log(level, "S3PutProperties@%p: contentEncoding=\"%s\"\n", properties, properties->contentEncoding); }
			logger.log(level, "S3PutProperties@%p: expires=%lld\n", properties, properties->expires);
			logger.log(level, "S3PutProperties@%p: cannedAcl=%d\n", properties, properties->cannedAcl);
			logger.log(level, "S3PutProperties@%p: metaDataCount=%d\n", properties, properties->metaDataCount);
			for (int ii = 0; ii < properties->metaDataCount; ++ii) {
				if ((properties->metaData[ii].name != 0) || (properties->metaData[ii].value != 0)) {
					logger.log(level, "S3PutProperties@%p: \"%s\"=\"%s\"\n", properties,
							(properties->metaData[ii].name != 0) ? properties->metaData[ii].name : "",
							(properties->metaData[ii].value != 0) ? properties->metaData[ii].value : "");
				}
			}
		}
	}
}

}
}
}
}
