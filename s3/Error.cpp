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

void show(const ::S3ErrorDetails * details) {
	if (details != 0) {
		Logger & logger = Logger::instance();
		if (details->message != 0) { logger.debug("Error: message=\"%s\"\n", details->message); }
		if (details->resource != 0) { logger.debug("Error: resource=\"%s\"\n", details->resource); }
		if (details->furtherDetails != 0) { logger.debug("Error: further=\"%s\"\n", details->furtherDetails); }
		if (details->extraDetails != 0) {
			for (int ii = 0; ii < details->extraDetailsCount; ++ii) {
				if ((details->extraDetails[ii].name != 0) || (details->extraDetails[ii].value != 0)) {
					logger.debug("Error: \"%s\"=\"%s\"\n",
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
