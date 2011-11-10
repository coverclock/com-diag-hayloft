/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Error.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void show(const ::S3ErrorDetails * details, Logger::Level level) {
	if (details != 0) {
		Logger & logger = Logger::instance();
		if (logger.isEnabled(level)) {
			if (details->message != 0) { logger.log(level, "Error@%p: message=\"%s\"\n", details, details->message); }
			if (details->resource != 0) { logger.log(level, "Error@%p: resource=\"%s\"\n", details, details->resource); }
			if (details->furtherDetails != 0) { logger.log(level, "Error@%p: further=\"%s\"\n", details, details->furtherDetails); }
			if (details->extraDetails != 0) {
				for (int ii = 0; ii < details->extraDetailsCount; ++ii) {
					if ((details->extraDetails[ii].name != 0) || (details->extraDetails[ii].value != 0)) {
						logger.log(level, "Error@%p: \"%s\"=\"%s\"\n", details,
								(details->extraDetails[ii].name != 0) ? details->extraDetails[ii].name : "",
								(details->extraDetails[ii].value != 0) ? details->extraDetails[ii].value : "");
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
