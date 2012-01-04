/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectCopy.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void ObjectCopy::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectCopy * that = static_cast<ObjectCopy*>(callbackData);
	if (that->entitytag[0] != '\0') { that->entitytag[sizeof(that->entitytag) - 1] = '\0'; that->etag = that->entitytag; }
	(*that->Object::handler.completeCallback)(status, errorDetails, callbackData);
}

ObjectCopy::ObjectCopy(const char * fromkeyname, const Bucket & frombucket, const char * tokeyname, const Bucket & tobucket, const Properties & props)
: Object(fromkeyname, frombucket)
, tocanonical(tobucket.getCanonical())
, tokey(tokeyname)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
{
	initialize(props.getMetadata());
	execute();
}

ObjectCopy::ObjectCopy(const char * fromkeyname, const Bucket & frombucket, const char * tokeyname, const Bucket & tobucket, const Plex & plex, const Properties & props)
: Object(fromkeyname, frombucket, plex)
, tocanonical(tobucket.getCanonical())
, tokey(tokeyname)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
{
	initialize(props.getMetadata());
}

ObjectCopy::ObjectCopy(const Object & fromobject, const Object & toobject, const Properties & props)
: Object(fromobject)
, tocanonical(toobject.getCanonical())
, tokey(toobject.getKey())
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
{
	initialize(props.getMetadata());
	execute();
}

ObjectCopy::ObjectCopy(const Object & fromobject, const Object & toobject, const Plex & plex, const Properties & props)
: Object(fromobject, plex)
, tocanonical(toobject.getCanonical())
, tokey(toobject.getKey())
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
{
	initialize(props.getMetadata());
}

ObjectCopy::~ObjectCopy() {
	if (isBusy() && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
	delete [] properties.metaData;
}

void ObjectCopy::initialize(const Properties::Metadata & settings) {
	state(static_cast<Status>(IDLE));
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("ObjectCopy@%p: tocanonical=\"%s\"\n", this, tocanonical.c_str());
		logger.debug("ObjectCopy@%p: tokey=\"%s\"\n", this, tokey.c_str());
	}
	entitytag[0] = '\0';
	std::memset(&properties, 0, sizeof(properties));
	properties.contentType = set(type);
	properties.md5 = set(checksum);
	properties.cacheControl = set(control);
	properties.contentDispositionFilename = set(filename);
	properties.contentEncoding = set(encoding);
	properties.expires = expires;
	properties.cannedAcl = access;
	properties.metaDataCount = settings.size();
	if (properties.metaDataCount > 0) {
		Properties::Metadata::const_iterator here;
		Properties::Metadata::const_iterator end;
		for (here = settings.begin(), end = settings.end(); here != end; ++here) {
			metadata.insert(Properties::Pair(here->first, here->second));
		}
		::S3NameValue * pair = new ::S3NameValue [properties.metaDataCount];
		properties.metaData = pair;
		for (here = metadata.begin(), end = metadata.end(); here != end; ++here, ++pair) {
			pair->name = (here->first).c_str();
			pair->value = (here->second).c_str();
		}
	}
	show(&properties, Logger::DEBUG);
	handler.propertiesCallback = Object::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void ObjectCopy::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("ObjectCopy@%p: begin\n", this);
	Object::execute();
	::S3_copy_object(
		&context,
		key.c_str(),
		tocanonical.c_str(),
		tokey.c_str(),
		&properties,
		&modified,
		sizeof(entitytag), entitytag,
		pending,
		&handler,
		this
	);
}

bool ObjectCopy::start(bool force) {
	if ((!isBusy()) || force) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
}
