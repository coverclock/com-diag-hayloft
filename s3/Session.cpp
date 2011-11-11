/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <ctype.h>
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Session::Session(const char * userAgentInfo, int flags, const char * defaultS3HostName)
: useragent(set(userAgentInfo, USER_AGENT_ENV(), USER_AGENT_STR()))
, hostname(set(defaultS3HostName, HOST_NAME_ENV(), HOST_NAME_STR()))
, status(::S3_initialize(useragent.c_str(), flags, hostname.c_str()))
{
	Logger & logger = Logger::instance();
	logger.setMask();
	logger.debug("Session@%p: useragent=\"%s\"\n", this, useragent.c_str());
	logger.debug("Session@%p: hostname=\"%s\"\n", this, hostname.c_str());
	if (status != ::S3StatusOK) {
		logger.error("Session@%p: S3_initialize failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Session::~Session() {
	S3_deinitialize();
}

const char * Session::canonicalize(std::string name) const {
	Logger & logger = Logger::instance();
	size_t namelength = name.length();
	logger.debug("Session@%p: name=\"%s\"[%zu]\n", this, name.c_str(), namelength);
	size_t useragentlength = useragent.length();
	if (useragentlength > 0) { ++useragentlength; /* '.' */ }
	ssize_t effectivelength = namelength + useragentlength;
	if (effectivelength > BUCKET_NAME_LEN) {
		effectivelength -= BUCKET_NAME_LEN;
		if (effectivelength < namelength)  {
			name = name.substr(0, namelength - effectivelength);
		}
	}
	if (useragentlength > 0) {
		name += '.';
		name += useragent;
	}
	for (int ii = 0; ii < name.length(); ++ii) {
		name.replace(ii, 1, 1, tolower(name[ii]));
	}
	logger.debug("Session@%p: canonical=\"%s\"[%zu]\n", this, name.c_str(), name.length());
	return name.c_str();
}

}
}
}
}
