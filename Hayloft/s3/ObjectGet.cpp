/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status ObjectGet::getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	::S3Status status = that->get(bufferSize, buffer);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	Logger::instance().log(level, "ObjectGet@%p: requested=%d status=%d=\"%s\"\n", that, bufferSize, status, ::S3_get_status_name(status));
	return status;
}

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, ::com::diag::desperado::Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
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

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, ::com::diag::desperado::Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
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

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, Multiplex & multiplex, ::com::diag::desperado::Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket, multiplex)
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

ObjectGet::ObjectGet(const char * keyname, const Bucket & bucket, Multiplex & multiplex, ::com::diag::desperado::Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(keyname, bucket, multiplex)
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
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
	if (taken != 0) {
		delete taken;
	}
}


void ObjectGet::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	std::memset(&conditions, 0, sizeof(conditions));
	conditions.ifModifiedSince = since;
	conditions.ifNotModifiedSince = notsince;
	conditions.ifMatchETag = match.empty() ? 0 : match.c_str();
	conditions.ifNotMatchETag = notmatch.empty() ? 0 : notmatch.c_str();
	show(&conditions);
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Object::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Object::handler.completeCallback;
	handler.getObjectDataCallback = &getObjectDataCallback;
}

void ObjectGet::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectGet@%p: begin\n", this);
	::S3_get_object(
		&context,
		key.c_str(),
		&conditions,
		offset,
		size,
		requests,
		&handler,
		this
	);

}

::S3Status ObjectGet::get(int bufferSize, const char * buffer) {
	ssize_t produced = 0;
	if (output != 0) {
		produced = (*output)(buffer, bufferSize, bufferSize);
		if (produced == EOF) {
			produced = 0;
			output = 0;
		}
	}
	return (produced > 0) ? ::S3StatusOK : ::S3StatusAbortedByCallback;
}

void ObjectGet::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	if (output != 0) {
		(*output)();
	}
	Logger::instance().debug("ObjectGet@%p: end\n", this);
}

}
}
}
}
