/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Credentials.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/hayloft/Debug.h"

namespace com {
namespace diag {
namespace hayloft {

const size_t Credentials::ACCESS_KEY_ID_LEN;

const size_t Credentials::SECRET_ACCESS_KEY_LEN;

static const char EXS[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

const char * Credentials::obfuscate(const char * str) {
    if (Debugging) {
	    return str;
    } else {
	    return &EXS[sizeof(EXS) - 1 - ::strnlen(str, sizeof(EXS) - 1)];
    }
}

Credentials::Credentials(const char * accessKeyId, const char * secretAccessKey)
: id(::com::diag::grandote::set(accessKeyId, ACCESS_KEY_ID_ENV(), ""))
, secret(::com::diag::grandote::set(secretAccessKey, SECRET_ACCESS_KEY_ENV(), ""))
{
	initialize();
}

Credentials::~Credentials()
{
}

void Credentials::initialize() {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	logger.debug("Credentials@%p: id=\"%s\"[%zu]\n", this, obfuscate(id.c_str()), id.length());
	if (id.length() != ACCESS_KEY_ID_LEN) {
		logger.warning("Credentials@%p: access key id length invalid! (%zu!=%zu)\n", this, id.length(), ACCESS_KEY_ID_LEN);
	}
	logger.debug("Credentials@%p: secret=\"%s\"[%zu]\n", this, obfuscate(secret.c_str()), secret.length());
	if (secret.length() != SECRET_ACCESS_KEY_LEN) {
		logger.warning("Credentials@%p: secret access key length invalid! (%zu!=%zu)\n", this, secret.length(), SECRET_ACCESS_KEY_LEN);
	}
}

}
}
}
