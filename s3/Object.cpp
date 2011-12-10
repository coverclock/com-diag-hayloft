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
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status Object::responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData) {
	Object * that = static_cast<Object*>(callbackData);
	::S3Status status = (*that->Container::handler.propertiesCallback)(responseProperties, callbackData);
	if (status == ::S3StatusOK) {
		if ((responseProperties->contentType != 0) && (responseProperties->contentType[0] != '\0')) { that->type = responseProperties->contentType; }
		that->length = responseProperties->contentLength;
		if ((responseProperties->eTag != 0) && (responseProperties->eTag[0] != '\0')) { that->etag = responseProperties->eTag; }
		that->modified = responseProperties->lastModified;
		if (responseProperties->metaData != 0) {
			for (int ii = 0; ii < responseProperties->metaDataCount; ++ii) {
				if (
					(responseProperties->metaData[ii].name != 0) &&
					(responseProperties->metaData[ii].name[0] != '\0') &&
					(responseProperties->metaData[ii].value != 0)
				) {
					that->metadata.insert(Pair(responseProperties->metaData[ii].name, responseProperties->metaData[ii].value));
				}
			}
		}
	}
	return status;
}

Object::Object()
: Container("", "", "", "", Protocol::DEFAULT, Style::DEFAULT)
, length(0)
, modified(-1)
{
	initialize();
}

Object::Object(const char * keyname, const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, key(keyname)
, length(0)
, modified(-1)
{
	initialize();
}

Object::Object(const char * keyname, const Bucket & bucket, const Plex & plex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), plex)
, key(keyname)
, length(0)
, modified(-1)
{
	initialize();
}

Object::Object(const Object & object)
: Container(object.getId(), object.getSecret(), object.getEndpoint(), object.getCanonical(), object.getProtocol(), object.getStyle())
, key(object.getKey())
, type(object.getContentType())
, etag(object.getETag())
, length(object.getContentLength())
, modified(object.getModificationTime())
{
	initialize();
}

Object::Object(const Object & object, const Plex & plex)
: Container(object.getId(), object.getSecret(), object.getEndpoint(), object.getCanonical(), object.getProtocol(), object.getStyle(), plex)
, key(object.getKey())
, type(object.getContentType())
, etag(object.getETag())
, length(object.getContentLength())
, modified(object.getModificationTime())
{
	initialize();
}

Object::~Object() {}

void Object::initialize() {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Object@%p: key=\"%s\"\n", this, key.c_str());
	}
	std::memset(&handler, 0, sizeof(handler));
	handler.propertiesCallback = &responsePropertiesCallback;
	handler.completeCallback = Container::handler.completeCallback;
}

const char * Object::find(const char * key) const {
	const char * value = 0;
	Metadata::const_iterator here = metadata.find(key);
	if (here != metadata.end()) {
		value = (here->second).c_str();
	}
	return value;
}

const char * Object::authenticated(Epochalseconds expires, const char * resource) {
	const char * result = 0;
	char buffer[S3_MAX_AUTHENTICATED_QUERY_STRING_SIZE + 1];
	::S3Status status = S3_generate_authenticated_query_string(buffer, &context, key.c_str(), expires, resource);
	if (status == ::S3StatusOK) {
		buffer[sizeof(buffer) - 1] = '\0';
		authentic = buffer;
		result = authentic.c_str();
	} else {
		Logger::instance().error("Object@%p: S3_generate_authenticated_query_string failed! status=%d=\"%s\"\n", this, status, tostring(status));
	}
	return result;
}

}
}
}
}
