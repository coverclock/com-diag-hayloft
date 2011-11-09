/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const size_t Credentials::ACCESS_KEY_ID_LEN;

const size_t Credentials::SECRET_ACCESS_KEY_LEN;

Credentials::Credentials(const char * accessKeyId, const char * secretAccessKey)
: id(set(accessKeyId, ACCESS_KEY_ID_ENV(), ""))
, secret(set(secretAccessKey, SECRET_ACCESS_KEY_ENV(), ""))
{
	audit();
}

Credentials::Credentials(::com::diag::desperado::Input & accessKeyIdIn, ::com::diag::desperado::Input & secretAccessKeyIn)
{
	{
		char val[ACCESS_KEY_ID_LEN + sizeof("\n")];
		size_t len = accessKeyIdIn(val, sizeof(val));
		if (len > 0) {
			if (val[len - 2] == '\n') { val[len - 2] = '\0'; }
			id = val;
		}
	}
	{
		char val[SECRET_ACCESS_KEY_LEN + sizeof("\n")];
		size_t len = secretAccessKeyIn(val, sizeof(val));
		if (len > 0) {
			if (val[len - 2] == '\n') { val[len - 2] = '\0'; }
			secret = val;
		}
	}
	audit();
}

Credentials::~Credentials()
{
}

bool Credentials::audit() {
	bool result = true;
#if defined(NDEBUG)
	Logger::instance().debug("Credentials@%p: id=[%zu]\n", this, id.length());
#else
	Logger::instance().debug("Credentials@%p: id=\"%s\"[%zu]\n", this, id.c_str(), id.length());
#endif
	if (id.length() != ACCESS_KEY_ID_LEN) {
		Logger::instance().warning("Credentials@%p: access key id length invalid! (%zu!=%zu)\n", this, id.length(), ACCESS_KEY_ID_LEN);
		result = false;
	}
#if defined(NDEBUG)
	Logger::instance().debug("Credentials@%p: secret=[%zu]\n", this, secret.length());
#else
	Logger::instance().debug("Credentials@%p: secret=\"%s\"[%zu]\n", this, secret.c_str(), secret.length());
#endif
	if (secret.length() != SECRET_ACCESS_KEY_LEN) {
		Logger::instance().warning("Credentials@%p: secret access key length invalid! (%zu!=%zu)\n", this, secret.length(), SECRET_ACCESS_KEY_LEN);
		result = false;
	}
	return result;
}

}
}
}
}
