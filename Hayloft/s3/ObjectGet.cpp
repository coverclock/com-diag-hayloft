/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Status ObjectGet::getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	Status status = that->get(bufferSize, buffer);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	Logger::instance().log(level, "ObjectGet@%p: bufferSize=%d\n", that, bufferSize);
	Logger::instance().log(level, "ObjectGet@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	return status;
}

void ObjectGet::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	that->finalize();
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
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
, size(objectsize)
{
	initialize();
}

ObjectGet::~ObjectGet() {
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
	finalize();
}

void ObjectGet::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
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
	status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectGet@%p: begin\n", this);
	LifeCycle::instance().start(*this);
	::S3_get_object(
		&context,
		key.c_str(),
		&conditions,
		offset,
		size,
		pending,
		&handler,
		this
	);
}

void ObjectGet::finalize() {
	if (output != 0) {
		(*output)();
		output = 0;
	}
	delete taken;
	taken = 0;
}

void ObjectGet::start() {
	if (state() != BUSY) {
		execute();
	}
}

void ObjectGet::reset(Output & sink, Octets objectoffset, Octets objectsize) {
	if ((state() != BUSY)) {
		finalize();
		output = &sink;
		taken = 0;
		offset = objectoffset;
		size = objectsize;
	}
}

void ObjectGet::reset(Output * sinkp /* TAKEN */, Octets objectoffset, Octets objectsize) {
	if ((state() != BUSY)) {
		finalize();
		output = sinkp;
		taken = sinkp;
		offset = objectoffset;
		size = objectsize;
	}
}

Status ObjectGet::get(int bufferSize, const void * buffer) {
	ssize_t produced = 0;
	if (output != 0) {
		produced = (*output)(buffer, bufferSize, bufferSize);
		if (produced == EOF) {
			finalize();
			produced = 0;
		}
	}
	return (produced > 0) ? ::S3StatusOK : ::S3StatusAbortedByCallback;
}

}
}
}
}
