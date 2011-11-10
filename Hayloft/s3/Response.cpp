/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Response.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void show(const ::S3ResponseProperties * properties, Logger::Level level) {
	if (details != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (details->requestId != 0) { logger.log(level, "Response@%p: requestId=\"%s\"\n", properties, properties->requestId); }
			if (details->requestId2 != 0) { logger.log(level, "Response@%p: requestId2=\"%s\"\n", properties, properties->requestId2); }
			if (details->contentType != 0) { logger.log(level, "Response@%p: contentType=\"%s\"\n", properties, properties->contentType); }
			if (details->contentLength != 0) { logger.log(level, "Response@%p: contentLength=%llu\n", properties, properties->contentLength); }
			if (details->server != 0) { logger.log(level, "Response@%p: server=\"%s\"\n", properties, properties->server); }
			if (details->eTag != 0) { logger.log(level, "Response@%p: eTag=\"%s\"\n", properties, properties->eTag); }
			if (details->lastModified != 0) { logger.log(level, "Response@%p: lastModified=%lld\n", properties, properties->lastModified); }
			if (details->metaData != 0) {
				for (int ii = 0; ii < properties->metaDataCount; ++ii) {
					if ((properties->metaData[ii].name != 0) || (properties->metaData[ii].value != 0)) {
						logger.log(level, "Response@%p: \"%s\"=\"%s\"\n", properties,
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
}
