/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/errno.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

int ObjectPut::putObjectDataCallback(int bufferSize, char * buffer, void * callbackData) {
	ObjectPut * that = static_cast<ObjectPut*>(callbackData);
	// Remember, this is actually a read; put is the S3 operation in progress.
	int rc = that->put(bufferSize, buffer);
	if (rc >= 0) {
		that->consumed += rc;
		Logger::instance().debug("ObjectPut@%p: requested=%d returned=%d total=%d\n", that, bufferSize, rc, that->consumed);
	} else {
		Logger::instance().notice("ObjectPut@%p: requested=%d returned=%d total=%d errno=%d=\"%s\"\n", that, bufferSize, rc, that->consumed, errno, ::strerror(errno));
		rc = 0;
	}
	return (rc > 0) ? rc : 0;
}

void ObjectPut::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectPut * that = static_cast<ObjectPut*>(callbackData);
	that->finalize();
	(*that->Object::handler.completeCallback)(status, errorDetails, callbackData);
}

ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, Input & source, Octets objectsize, const Properties & props)
: Object(keyname, bucket)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(&source)
, taken(0)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
	execute();
}

ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, Input * sourcep, Octets objectsize, const Properties & props)
: Object(keyname, bucket)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(sourcep)
, taken(sourcep)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
	execute();
}
ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, const Plex & plex, Input & source, Octets objectsize, const Properties & props)
: Object(keyname, bucket, plex)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(&source)
, taken(0)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
}

ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, const Plex & plex, Input * sourcep, Octets objectsize, const Properties & props)
: Object(keyname, bucket, plex)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(sourcep)
, taken(sourcep)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
}

ObjectPut::ObjectPut(const Object & object, Input & source, Octets objectsize, const Properties & props)
: Object(object)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(&source)
, taken(0)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
	execute();
}

ObjectPut::ObjectPut(const Object & object, Input * sourcep, Octets objectsize, const Properties & props)
: Object(object)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(sourcep)
, taken(sourcep)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
	execute();
}
ObjectPut::ObjectPut(const Object & object, const Plex & plex, Input & source, Octets objectsize, const Properties & props)
: Object(object, plex)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(&source)
, taken(0)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
}

ObjectPut::ObjectPut(const Object & object, const Plex & plex, Input * sourcep, Octets objectsize, const Properties & props)
: Object(object, plex)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, expires(props.getExpires())
, access(props.getAccess())
, input(sourcep)
, taken(sourcep)
, size(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
}

ObjectPut::~ObjectPut() {
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
	delete [] properties.metaData;
	finalize();
}

void ObjectPut::initialize(const Properties::Metadata & settings) {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
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
			metadata.insert(Pair(here->first, here->second));
		}
		::S3NameValue * pair = new ::S3NameValue [properties.metaDataCount];
		properties.metaData = pair;
		for (here = metadata.begin(), end = metadata.end(); here != end; ++here, ++pair) {
			pair->name = (here->first).c_str();
			pair->value = (here->second).c_str();
		}
	}
	show(&properties, Logger::DEBUG);
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Object::handler.propertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;;
	handler.putObjectDataCallback = &putObjectDataCallback;
}

void ObjectPut::execute() {
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectPut@%p: begin\n", this);
	Object::execute();
	::S3_put_object(
		&context,
		key.c_str(),
		size,
		&properties,
		pending,
		&handler,
		this
	);
}

void ObjectPut::finalize() {
	if (input != 0) {
		input = 0;
	}
	// For some input functors, the delete performs the close on the underlying
	// operating system data source.
	delete taken;
	taken = 0;
}

void ObjectPut::start() {
	if (state() != BUSY) {
		execute();
	}
}

void ObjectPut::reset(Input & source, Octets objectsize) {
	if ((state() != BUSY)) {
		finalize();
		input = &source;
		taken = 0;
		size = objectsize;
		consumed = 0;
	}
}

void ObjectPut::reset(Input * sourcep /* TAKEN */, Octets objectsize) {
	if ((state() != BUSY)) {
		finalize();
		input = sourcep;
		taken = sourcep;
		size = objectsize;
		consumed = 0;
	}
}

int ObjectPut::put(int bufferSize, void * buffer) {
	ssize_t octets = 0;
	if (input != 0) {
		octets = (*input)(buffer, 1, bufferSize);
		if (octets == EOF) {
			finalize();
			octets = 0;
		}
	}
	return octets;
}

}
}
}
}
