/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/grandote/Output.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/errno.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Status ObjectGet::getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	// Remember, this is actually a write; get is the S3 operation in progress.
	int rc = that->get(bufferSize, buffer);
	if (rc > 0) { that->produced += rc; }
	Logger::instance().debug("ObjectGet@%p: requested=%d returned=%d total=%d\n", that, bufferSize, rc, that->produced);
	return (rc > 0) ? ::S3StatusOK : ::S3StatusAbortedByCallback;
}

void ObjectGet::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	if (!that->retryable(status)) { that->finalize(); }
	(*that->Object::handler.completeCallback)(status, errorDetails, callbackData);
}

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(&sink)
, taken(0)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
	execute();
}

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(sinkp)
, taken(sinkp)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
	execute();
}

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, const Plex & plex, Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket, plex)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(&sink)
, taken(0)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
}

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, const Plex & plex, Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket, plex)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(sinkp)
, taken(sinkp)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
}

ObjectGet::ObjectGet(const Object & object, Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(object)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(&sink)
, taken(0)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
	execute();
}

ObjectGet::ObjectGet(const Object & object, Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(object)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(sinkp)
, taken(sinkp)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
	execute();
}

ObjectGet::ObjectGet(const Object & object, const Plex & plex, Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(object, plex)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(&sink)
, taken(0)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
}

ObjectGet::ObjectGet(const Object & object, const Plex & plex, Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(object, plex)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, output(sinkp)
, taken(sinkp)
, offset(objectoffset)
, total(objectsize)
, produced(0)
{
	initialize();
}

ObjectGet::~ObjectGet() {
	finalize();
}

void ObjectGet::initialize() {
	state(static_cast<Status>(IDLE));
	std::memset(&conditions, 0, sizeof(conditions));
	conditions.ifModifiedSince = since;
	conditions.ifNotModifiedSince = notsince;
	conditions.ifMatchETag = set(match);
	conditions.ifNotMatchETag = set(notmatch);
	show(&conditions, Logger::DEBUG);
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Object::handler.propertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;
	handler.getObjectDataCallback = &getObjectDataCallback;
}

void ObjectGet::execute() {
	state(static_cast<Status>(BUSY));
	Logger::instance().debug("ObjectGet@%p: begin\n", this);
	Object::execute();
	::S3_get_object(
		&context,
		key.c_str(),
		&conditions,
		offset,
		total,
		handle,
		&handler,
		this
	);
}

void ObjectGet::finalize() {
	if (output != 0) { (*output)(); }
	output = 0;
	// For some output functors, the delete performs the close on the underlying
	// operating system data sink.
	delete taken;
	taken = 0;
}

bool ObjectGet::start() {
	if (Object::start()) {
		execute();
		return true;
	} else {
		return false;
	}
}

bool ObjectGet::reset() {
	return (Object::reset() && (produced == 0));
}

bool ObjectGet::reset(Output & sink, Octets objectoffset, Octets objectsize) {
	if (Object::reset()) {
		finalize();
		output = &sink;
		taken = 0;
		offset = objectoffset;
		total = objectsize;
		produced = 0;
		return true;
	} else {
		return false;
	}
}

bool ObjectGet::reset(Output * sinkp /* TAKEN */, Octets objectoffset, Octets objectsize) {
	if (Object::reset()) {
		finalize();
		output = sinkp;
		taken = sinkp;
		offset = objectoffset;
		total = objectsize;
		produced = 0;
		return true;
	} else {
		return false;
	}
}

int ObjectGet::get(int bufferSize, const void * buffer) {
	ssize_t octets = 0;
	if (output != 0) {
		octets = (*output)(buffer, bufferSize, bufferSize);
		if (octets == EOF) {
			if (errno != 0) { Logger::instance().error("ObjectGet@%p: failed! errno=%d=\"%s\"\n", this, errno, ::strerror(errno)); }
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
