/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const size_t Credentials::ACCESS_KEY_ID_LEN;

const size_t Credentials::SECRET_ACCESS_KEY_LEN;

static const char EXS[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

const char * Credentials::obfuscate(const char * str) {
#if defined(NDEBUG)
   return &EXS[sizeof(EXS) - 1 - ::strnlen(str, sizeof(EXS) - 1)];
#else
	return str;
#endif
}

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
	Logger & logger = Logger::instance();
	bool result = true;
	logger.debug("Credentials@%p: id=\"%s\"[%zu]\n", this, obfuscate(id.c_str()), id.length());
	if (id.length() != ACCESS_KEY_ID_LEN) {
		logger.warning("Credentials@%p: access key id length invalid! (%zu!=%zu)\n", this, id.length(), ACCESS_KEY_ID_LEN);
		result = false;
	}
	logger.debug("Credentials@%p: secret=\"%s\"[%zu]\n", this, obfuscate(secret.c_str()), secret.length());
	if (secret.length() != SECRET_ACCESS_KEY_LEN) {
		logger.warning("Credentials@%p: secret access key length invalid! (%zu!=%zu)\n", this, secret.length(), SECRET_ACCESS_KEY_LEN);
		result = false;
	}
	return result;
}

}
}
}
}
