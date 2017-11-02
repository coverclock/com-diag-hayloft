/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Service.h"
#include "com/diag/hayloft/Credentials.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

Service::Service(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, ::S3Protocol httpProtocol)
: Action()
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, protocol(httpProtocol)
{
	initialize();
}

Service::Service(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, ::S3Protocol httpProtocol, const Plex & plex)
: Action(plex)
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, protocol(httpProtocol)
{
	initialize();
}

Service::~Service() {}

void Service::initialize() {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("Service@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
		logger.debug("Service@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
		logger.debug("Service@%p: endpoint=\"%s\"\n", this, endpoint.c_str());
		logger.debug("Service@%p: protocol=%d=\"%s\"\n", this, protocol, tostring(protocol));
	}
}

}
}
}
