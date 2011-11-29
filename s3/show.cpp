/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void show(const ::S3ResponseProperties * response, Logger::Level level) {
	if (response != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (response->requestId != 0) { logger.log(level, "S3ResponseProperties@%p: requestId=\"%s\"\n", response, response->requestId); }
			if (response->requestId2 != 0) { logger.log(level, "S3ResponseProperties@%p: requestId2=\"%s\"\n", response, response->requestId2); }
			if (response->contentType != 0) { logger.log(level, "S3ResponseProperties@%p: contentType=\"%s\"\n", response, response->contentType); }
			if (response->contentLength > 0) { logger.log(level, "S3ResponseProperties@%p: contentLength=%llu\n", response, response->contentLength); }
			if (response->server != 0) { logger.log(level, "S3ResponseProperties@%p: server=\"%s\"\n", response, response->server); }
			if (response->eTag != 0) { logger.log(level, "S3ResponseProperties@%p: eTag=\"%s\"\n", response, response->eTag); }
			if (response->lastModified >= 0) { logger.log(level, "S3ResponseProperties@%p: lastModified=%lld\n", response, response->lastModified); }
			if (response->metaData != 0) {
				for (int ii = 0; ii < response->metaDataCount; ++ii) {
					if ((response->metaData[ii].name != 0) || (response->metaData[ii].value != 0)) {
						logger.log(level, "S3ResponseProperties@%p: \"%s\"=\"%s\"\n", response,
								(response->metaData[ii].name != 0) ? response->metaData[ii].name : "",
								(response->metaData[ii].value != 0) ? response->metaData[ii].value : "");
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
			logger.log(level, "S3BucketContext@%p: protocol=%d\n", context, context->protocol);
			logger.log(level, "S3BucketContext@%p: uriStyle=%d\n", context, context->uriStyle);
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
			if (properties->expires >= 0) { logger.log(level, "S3PutProperties@%p: expires=%lld\n", properties, properties->expires); }
			logger.log(level, "S3PutProperties@%p: cannedAcl=%d\n", properties, properties->cannedAcl);
			if (properties->metaData != 0) {
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

void show(const ::S3GetConditions * conditions, Logger::Level level) {
	if (conditions != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (conditions->ifModifiedSince >= 0) { logger.log(level, "S3GetConditions@%p: ifModifiedSince=%lld\n", conditions, conditions->ifModifiedSince); }
			if (conditions->ifNotModifiedSince >= 0) { logger.log(level, "S3GetConditions@%p: ifNotModifiedSince=%lld\n", conditions, conditions->ifNotModifiedSince); }
			if (conditions->ifMatchETag != 0) { logger.log(level, "S3GetConditions@%p: ifMatchETag=\"%s\"\n", conditions, conditions->ifMatchETag); }
			if (conditions->ifNotMatchETag != 0) { logger.log(level, "S3GetConditions@%p: ifNotMatchETag=\"%s\"\n", conditions, conditions->ifNotMatchETag); }
		}
	}
}

void show(const Object & object, Logger::Level level) {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(level)) {
		logger.log(level, "Object%p: key=\"%s\"\n", &object, object.getKey());
		logger.log(level, "Object%p: name=\"%s\"\n", &object, object.getName());
		logger.log(level, "Object%p: canonical=\"%s\"\n", &object, object.getCanonical());
		logger.log(level, "Object%p: server=\"%s\"\n", &object, object.getServer());
		logger.log(level, "Object%p: requestId=\"%s\"\n", &object, object.getRequestId());
		logger.log(level, "Object%p: requestId2=\"%s\"\n", &object, object.getRequestId2());
		logger.log(level, "Object%p: contentType=\"%s\"\n", &object, object.getContentType());
		logger.log(level, "Object%p: eTag=\"%s\"\n", &object, object.getETag());
		logger.log(level, "Object%p: contentLength=%llu\n", &object, object.getContentLength());
		logger.log(level, "Object%p: modificationTime=%lld\n", &object, object.getModificationTime());
		Object::Metadata::const_iterator here = object.getMetadata().begin();
		Object::Metadata::const_iterator there = object.getMetadata().end();
		while (here != there) {
			logger.log(level, "Object%p: \"%s\"=\"%s\"\n", &object, here->first.c_str(), here->second.c_str());
			++here;
		}
	}
}

}
}
}
}
