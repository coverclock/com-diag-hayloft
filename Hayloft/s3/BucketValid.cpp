/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/BucketValid.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketValid::BucketValid(const Session & se, const char * na, const Context & co)
: Bucket(se, na, co)
{
	validate();
}

void BucketValid::validate() {
	Logger & logger = Logger::instance();
	logger.debug("BucketValid@%p: begin\n", this);
	status = ::S3_validate_bucket_name(name.c_str(), context.getStyle());
	if (status != ::S3StatusOK) {
		logger.warning("BucketValid@%p: S3_validate_bucket_name failed! name=\"%s\"[%zu] status=%d=\"%s\"\n", this, name.c_str(), name.length(), status, ::S3_get_status_name(status));
	}
	logger.debug("BucketValid@%p: end\n", this);
}

}
}
}
}
