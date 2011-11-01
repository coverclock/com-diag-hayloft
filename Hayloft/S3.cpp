/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 */

#include "com/diag/hayloft/S3.h"
#include "com/diag/hayloft/Logger.h"
#include <sys/utsname.h>
#include <cstdlib>
#include <cstring>

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/*******************************************************************************
 * SESSION
 ******************************************************************************/

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

/*******************************************************************************
 * CREDENTIALS
 ******************************************************************************/

Credentials::Credentials(const char * accessKeyId, const char * secretAccessKey)
{
	if (accessKeyId == 0) {
		accessKeyId = std::getenv(ACCESS_KEY_ID_ENV());
	}

	if (accessKeyId != 0) {
		id = accessKeyId;
	}

	if (secretAccessKey == 0) {
		secretAccessKey = std::getenv(SECRET_ACCESS_KEY_ENV());
	}

	if (secretAccessKey != 0) {
		secret = secretAccessKey;
	}

	Logger::instance().information("Credentials: id=[%zu]\n", id.length());
	Logger::instance().information("Credentials: secret=[%zu]\n", secret.length());

	if (id.length() != ACCESS_KEY_ID_LEN) {
		Logger::instance().warning("Credentials: access key id length invalid! (%zu!=%zu)\n", id.length(), ACCESS_KEY_ID_LEN);
	}

	if (secret.length() != SECRET_ACCESS_KEY_LEN) {
		Logger::instance().warning("Credentials: secret access key length invalid! (%zu!=%zu)\n", secret.length(), SECRET_ACCESS_KEY_LEN);
	}
}

Credentials::~Credentials()
{
}

}
}
}
}
