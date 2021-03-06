/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/errno.h"

namespace com {
namespace diag {
namespace hayloft {

int ObjectPut::putObjectDataCallback(int bufferSize, char * buffer, void * callbackData) {
	ObjectPut * that = static_cast<ObjectPut*>(callbackData);
	// Remember, this is actually a read; put is the S3 operation in progress.
	int rc = that->put(bufferSize, buffer);
	if (rc > 0) { that->consumed += rc; }
	::com::diag::grandote::MaskableLogger::instance().debug("ObjectPut@%p: requested=%d returned=%d total=%d\n", that, bufferSize, rc, that->consumed);
	return rc;
}

void ObjectPut::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectPut * that = static_cast<ObjectPut*>(callbackData);
	if (!that->retryable(status)) { that->finalize(); }
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
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
, total(objectsize)
, consumed(0)
{
	initialize(props.getMetadata());
}

ObjectPut::~ObjectPut() {
	delete [] properties.metaData;
	finalize();
}

void ObjectPut::initialize(const Properties::Metadata & settings) {
	state(static_cast<Status>(IDLE));
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
			metadata.insert(Pair(here->first, here->second));
		}
		::S3NameValue * pair = new ::S3NameValue [properties.metaDataCount];
		properties.metaData = pair;
		for (here = metadata.begin(), end = metadata.end(); here != end; ++here, ++pair) {
			pair->name = (here->first).c_str();
			pair->value = (here->second).c_str();
		}
	}
	show(&properties, ::com::diag::grandote::MaskableLogger::DEBUG);
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Object::handler.propertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;;
	handler.putObjectDataCallback = &putObjectDataCallback;
}

void ObjectPut::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("ObjectPut@%p: begin\n", this);
	Object::execute();
	::S3_put_object(
		&context,
		key.c_str(),
		total,
		&properties,
		handle,
		&handler,
		this
	);
}

void ObjectPut::finalize() {
	input = 0;
	// For some input functors, the delete performs the close on the underlying
	// operating system data source.
	delete taken;
	taken = 0;
}

bool ObjectPut::start() {
	if (Object::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

bool ObjectPut::reset() {
	return (Object::reset() && (consumed == 0));
}

bool ObjectPut::reset(Input & source, Octets objectsize) {
	if (Object::reset()) {
		finalize();
		input = &source;
		taken = 0;
		total = objectsize;
		consumed = 0;
		return true;
	} else {
		return false;
	}
}

bool ObjectPut::reset(Input * sourcep /* TAKEN */, Octets objectsize) {
	if (Object::reset()) {
		finalize();
		input = sourcep;
		taken = sourcep;
		total = objectsize;
		consumed = 0;
		return true;
	} else {
		return false;
	}
}

int ObjectPut::put(int bufferSize, void * buffer) {
	ssize_t octets = 0;
	if (input != 0) {
		octets = (*input)(buffer, 1, bufferSize);
		if (octets == EOF) {
			if (errno != 0) { ::com::diag::grandote::MaskableLogger::instance().error("ObjectPut@%p: failed! errno=%d=\"%s\"\n", this, errno, ::strerror(errno)); }
			finalize();
			octets = 0;
		}
	}
	return octets;
}

}
}
}
