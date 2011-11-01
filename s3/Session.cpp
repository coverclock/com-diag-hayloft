/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 */

#include <cstdlib>
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Session::Session(const char * userAgentInfo, int flags, const char * defaultS3HostName)
: status(S3StatusOK)
{
	if (userAgentInfo == 0) {
		userAgentInfo = std::getenv(USER_AGENT_ENV());
	}

	if (userAgentInfo == 0) {
		userAgentInfo = USER_AGENT_STR();
	}

	useragent = userAgentInfo;

	if (defaultS3HostName == 0) {
		defaultS3HostName = std::getenv(HOST_NAME_ENV());
	}

	if (defaultS3HostName == 0) {
		defaultS3HostName = HOST_NAME_STR();
	}

	hostname = defaultS3HostName;

	Logger::instance().information("Session: useragent=\"%s\"\n", useragent.c_str());
	Logger::instance().information("Session: hostname=\"%s\"\n", hostname.c_str());

	status = ::S3_initialize(useragent.c_str(), flags, hostname.c_str());
	if (status != ::S3StatusOK) {
		Logger::instance().error("Session: S3_initialize failed! status=%d=\"%s\"\n", status, ::S3_get_status_name(status));
	}
}

Session::~Session() {
	S3_deinitialize();
}

}
}
}
}
