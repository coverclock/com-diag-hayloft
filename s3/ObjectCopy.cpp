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
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

void ObjectCopy::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectCopy * that = static_cast<ObjectCopy*>(callbackData);
	that->tag[sizeof(tag) - 1] = '\0';
	that->etag = that->tag;
	(*that->Object::handler.completeCallback)(status, errorDetails, callbackData);
}

ObjectCopy::ObjectCopy(const char * fromkeyname, const Bucket & frombucket, const char * tokeyname, const Bucket & tobucket, const Properties & props)
: Object(fromkeyname, frombucket)
, tocanonical(tobucket.getCanonical())
, toname(tobucket.getName())
, tokey(tokeyname)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, modified(-1)
{
	initialize(props.getMetadata());
	execute();
}

ObjectCopy::ObjectCopy(const char * fromkeyname, const Bucket & frombucket, const char * tokeyname, const Bucket & tobucket, Multiplex & multiplex, const Properties & props)
: Object(fromkeyname, frombucket, multiplex)
, tocanonical(tobucket.getCanonical())
, toname(tobucket.getName())
, tokey(tokeyname)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, modified(-1)
{
	initialize(props.getMetadata());
	execute();
}

ObjectCopy::~ObjectCopy() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
	if (properties.metaData != 0) {
		delete [] properties.metaData;
	}
}

void ObjectCopy::initialize(const Properties::Metadata & settings) {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("ObjectCopy@%p: tocanonical=\"%s\"\n", this, tocanonical.c_str());
		logger.debug("ObjectCopy@%p: toname=\"%s\"\n", this, toname.c_str());
		logger.debug("ObjectCopy@%p: tokey=\"%s\"\n", this, tokey.c_str());
	}
	tag[0] = '\0';
	std::memset(&properties, 0, sizeof(properties));
	properties.contentType = type.empty() ? 0 : type.c_str();
	properties.md5 = checksum.empty() ? 0 : checksum.c_str();
	properties.cacheControl = control.empty() ? 0 : control.c_str();
	properties.contentDispositionFilename = filename.empty() ? 0 : filename.c_str();
	properties.contentEncoding = encoding.empty() ? 0 : encoding.c_str();
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
	show(&properties);
	handler.propertiesCallback = Object::handler.propertiesCallback;
	handler.completeCallback = &responseCompleteCallback;;
}

void ObjectCopy::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectCopy@%p: begin\n", this);
	::S3_copy_object(
		&context,
		key.c_str(),
		tocanonical.c_str(),
		tokey.c_str(),
		&properties,
		&modified,
		sizeof(tag), tag,
		requests,
		&handler,
		this
	);
}

void ObjectCopy::start() {
	if (state() != BUSY) {
		execute();
	}
}

}
}
}
}
