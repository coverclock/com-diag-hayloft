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
#include <cstring>
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Credentials::Credentials(const char * accessKeyId, const char * secretAccessKey)
{
	if (accessKeyId == 0) {
		accessKeyId = std::getenv(ACCESS_KEY_ID_ENV());
	}

	// There is no default access key id.

	if (accessKeyId != 0) {
		id = accessKeyId;
	}

	if (secretAccessKey == 0) {
		secretAccessKey = std::getenv(SECRET_ACCESS_KEY_ENV());
	}

	// There is no default secret access key.

	if (secretAccessKey != 0) {
		secret = secretAccessKey;
	}

	// We don't log the actual values of the access key id and most especially
	// the secret access key since these are considered sensitive and the log
	// files (particular the system log) might not be. Instead we just log the
	// length of each. If they weren't set successfully, their length will be
	// zero.

	Logger::instance().debug("Credentials@%p: id=[%zu]\n", this, id.length());

	if (id.length() != ACCESS_KEY_ID_LEN) {
		Logger::instance().warning("Credentials@%p: access key id length invalid! (%zu!=%zu)\n", this, id.length(), ACCESS_KEY_ID_LEN);
	}

	Logger::instance().debug("Credentials@%p: secret=[%zu]\n", this, secret.length());

	if (secret.length() != SECRET_ACCESS_KEY_LEN) {
		Logger::instance().warning("Credentials@%p: secret access key length invalid! (%zu!=%zu)\n", this, secret.length(), SECRET_ACCESS_KEY_LEN);
	}
}

Credentials::~Credentials()
{
}

}
}
}
}
