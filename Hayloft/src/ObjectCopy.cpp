/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/ObjectCopy.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {

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
	delete [] properties.metaData;
}

void ObjectCopy::initialize(const Properties::Metadata & settings) {
	state(static_cast<Status>(IDLE));
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("ObjectCopy@%p: tocanonical=\"%s\"\n", this, tocanonical.c_str());
		logger.debug("ObjectCopy@%p: tokey=\"%s\"\n", this, tokey.c_str());
	}
	entitytag[0] = '\0';
	std::memset(&properties, 0, sizeof(properties));
	properties.contentType = ::com::diag::grandote::set(type);
	properties.md5 = ::com::diag::grandote::set(checksum);
	properties.cacheControl = ::com::diag::grandote::set(control);
	properties.contentDispositionFilename = ::com::diag::grandote::set(filename);
	properties.contentEncoding = ::com::diag::grandote::set(encoding);
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
	show(&properties, ::com::diag::grandote::MaskableLogger::DEBUG);
	handler.propertiesCallback = Object::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void ObjectCopy::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("ObjectCopy@%p: begin\n", this);
	Object::execute();
	::S3_copy_object(
		&context,
		key.c_str(),
		tocanonical.c_str(),
		tokey.c_str(),
		&properties,
		&modified,
		sizeof(entitytag), entitytag,
		handle,
		&handler,
		this
	);
}

bool ObjectCopy::start() {
	if (Object::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

}
}
}
