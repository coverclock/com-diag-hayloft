/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Container.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {


Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * canonicalBucketName, ::S3Protocol httpProtocol, ::S3UriStyle uriStyle)
: Service(accessKeyId, secretAccessKey, endPoint, httpProtocol)
, canonical(canonicalBucketName)
, style(uriStyle)
{
	initialize();
}

Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * canonicalBucketName, ::S3Protocol httpProtocol, ::S3UriStyle uriStyle, const Multiplex & multiplex)
: Service(accessKeyId, secretAccessKey, endPoint, httpProtocol, multiplex)
, canonical(canonicalBucketName)
, style(uriStyle)
{
	initialize();
}

Container::~Container() {}

void Container::initialize() {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Container@%p: canonical=\"%s\"\n", this, canonical.c_str());
		logger.debug("Container@%p: style=%d=\"%s\"\n", this, style, tostring(style));
	}
	std::memset(&context, 0, sizeof(context));
	context.hostName = endpoint.c_str();
	context.bucketName = canonical.c_str();
	context.protocol = protocol;
	context.uriStyle = style;
	context.accessKeyId = id.c_str();
	context.secretAccessKey = secret.c_str();
	show(&context, Logger::DEBUG);
}

}
}
}
}
