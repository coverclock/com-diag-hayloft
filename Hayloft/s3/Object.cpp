/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Object::responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	if ((responseProperties->contentType != 0) && (responseProperties->contentType[0] != '\0')) { that->type = responseProperties->contentType; }
	that->length = responseProperties->contentLength;
	if ((responseProperties->eTag != 0) && (responseProperties->eTag[0] != '\0')) { that->etag = responseProperties->eTag; }
	if (responseProperties->lastModified != -1) { that->modified = responseProperties->lastModified; }
	return (*that->Container::handler.propertiesCallback)(responseProperties, callbackData);
}

Object::Object(const char * keyname, const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, key(keyname)
, length(0)
, modified(-1)
{
	initialize();
}

Object::Object(const char * keyname, const Bucket & bucket, Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, key(keyname)
, length(0)
, modified(-1)
{
	initialize();
}

Object::~Object() {}

void Object::initialize() {
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = Container::handler.completeCallback;
}

}
}
}
}
