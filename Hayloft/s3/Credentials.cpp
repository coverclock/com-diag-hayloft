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

	if (accessKeyId != 0) {
		id = accessKeyId;
	}

	if (secretAccessKey == 0) {
		secretAccessKey = std::getenv(SECRET_ACCESS_KEY_ENV());
	}

	if (secretAccessKey != 0) {
		secret = secretAccessKey;
	}

	Logger::instance().information("Credentials@%p: begin id=[%zu] secret=[%zu]\n", this, id.length(), secret.length());

	if (id.length() != ACCESS_KEY_ID_LEN) {
		Logger::instance().warning("Credentials@%p: access key id length invalid! (%zu!=%zu)\n", this, id.length(), ACCESS_KEY_ID_LEN);
	}

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
