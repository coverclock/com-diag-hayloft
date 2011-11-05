/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 */

#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Credentials::Credentials(const char * accessKeyId, const char * secretAccessKey)
: id(set(accessKeyId, ACCESS_KEY_ID_ENV(), ""))
, secret(set(secretAccessKey, SECRET_ACCESS_KEY_ENV(), ""))
{
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
