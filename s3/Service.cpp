/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Service.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Service::Service(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, ::S3Protocol httpProtocol)
: Action()
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, protocol(httpProtocol)
{
	initialize();
}

Service::Service(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, ::S3Protocol httpProtocol, Multiplex & multiplex)
: Action(multiplex)
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, protocol(httpProtocol)
{
	initialize();
}

Service::~Service() {}

void Service::initialize() {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Service@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
		logger.debug("Service@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
		logger.debug("Service@%p: endpoint=\"%s\"\n", this, endpoint.c_str());
		logger.debug("Service@%p: protocol=%d\n", this, protocol);
	}
}

}
}
}
}
