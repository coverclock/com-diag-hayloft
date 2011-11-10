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
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketValid::BucketValid(Session & session, const char * name, const Context & context)
{
	Logger & logger = Logger::instance();
	std::string fullname = name;
	fullname += session.getBucketSuffix();
	logger.debug("BucketValid@%p: fullname=\"%s\"\n", this, fullname.c_str());
	status = ::S3_validate_bucket_name(fullname.c_str(), context.getStyle());
	if (status != ::S3StatusOK) {
		logger.warning("BucketValid@%p: S3_validate_bucket_name failed! name=\"%s\"[%zu] status=%d=\"%s\"\n", this, fullname.c_str(), fullname.length(), status, ::S3_get_status_name(status));
	}
}

}
}
}
}
