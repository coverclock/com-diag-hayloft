/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Container.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {


Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * bucketName, ::S3Protocol proto, ::S3UriStyle uristyle)
: Action()
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, name(bucketName)
, protocol(proto)
, style(uristyle)
{
	initialize();
}

Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * bucketName, ::S3Protocol proto, ::S3UriStyle uristyle, Multiplex & multiplex)
: Action(multiplex)
, id(accessKeyId)
, secret(secretAccessKey)
, endpoint(endPoint)
, name(bucketName)
, protocol(proto)
, style(uristyle)
{
	initialize();
}

Container::~Container() {}

void Container::initialize() {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Container@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
		logger.debug("Container@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
		logger.debug("Container@%p: endpoint=\"%s\"\n", this, endpoint.c_str());
		logger.debug("Container@%p: name=\"%s\"\n", this, name.c_str());
		logger.debug("Container@%p: protocol=%d\n", this, protocol);
		logger.debug("Container@%p: style=%d\n", this, style);
	}
	std::memset(&context, 0, sizeof(context));
	context.hostName = endpoint.c_str();
	context.bucketName = name.c_str();
	context.protocol = protocol;
	context.uriStyle = style;
	context.accessKeyId = id.c_str();
	context.secretAccessKey = secret.c_str();
	show(&context);
}

}
}
}
}
