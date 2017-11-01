/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Container.h"
#include "com/diag/hayloft/Credentials.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {


Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * canonicalBucketName, ::S3Protocol httpProtocol, ::S3UriStyle uriStyle)
: Service(accessKeyId, secretAccessKey, endPoint, httpProtocol)
, canonical(canonicalBucketName)
, style(uriStyle)
{
	initialize();
}

Container::Container(const char * accessKeyId, const char * secretAccessKey, const char * endPoint, const char * canonicalBucketName, ::S3Protocol httpProtocol, ::S3UriStyle uriStyle, const Plex & plex)
: Service(accessKeyId, secretAccessKey, endPoint, httpProtocol, plex)
, canonical(canonicalBucketName)
, style(uriStyle)
{
	initialize();
}

Container::~Container() {}

void Container::initialize() {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
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
	show(&context, ::com::diag::grandote::MaskableLogger::DEBUG);
}

}
}
}
