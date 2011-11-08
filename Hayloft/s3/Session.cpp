/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Session::Session(const char * userAgentInfo, const char * bucketSuffix, int flags, const char * defaultS3HostName)
: useragent(set(userAgentInfo, USER_AGENT_ENV(), USER_AGENT_STR()))
, bucketsuffix(set(bucketSuffix, BUCKET_SUFFIX_ENV(), BUCKET_SUFFIX_STR()))
, hostname(set(defaultS3HostName, HOST_NAME_ENV(), HOST_NAME_STR()))
, status(::S3_initialize(useragent.c_str(), flags, hostname.c_str()))
{
	Logger::instance().debug("Session@%p: useragent=\"%s\"\n", this, useragent.c_str());
	Logger::instance().debug("Session@%p: bucketsuffix=\"%s\"\n", this, bucketsuffix.c_str());
	Logger::instance().debug("Session@%p: hostname=\"%s\"\n", this, hostname.c_str());
	if (status != ::S3StatusOK) {
		Logger::instance().error("Session@%p: S3_initialize failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Session::~Session() {
	S3_deinitialize();
}

}
}
}
}
