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
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

::S3Status ObjectGet::getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData) {
	ObjectGet * that = static_cast<ObjectGet*>(callbackData);
	::S3Status status = that->get(bufferSize, buffer);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::WARNING;
	Logger::instance().log(level, "ObjectGet@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	return status;
}

ObjectGet::ObjectGet(const Bucket & bucket, const char * keyname, ::com::diag::desperado::Output & sink, Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(bucket, keyname)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, outputp(0)
, output(sink)
, offset(objectoffset)
, size(objectsize)
, produced(0)
{
	initialize();
	begin();
}

ObjectGet::ObjectGet(const Bucket & bucket, const char * keyname, ::com::diag::desperado::Output * sinkp, /* TAKEN */ Octets objectoffset, Octets objectsize, const Conditions & conds)
: Object(bucket, keyname)
, since(conds.getSince())
, notsince(conds.getNotSince())
, match(conds.getMatch())
, notmatch(conds.getNotMatch())
, outputp(sinkp)
, output(*sinkp)
, offset(objectoffset)
, size(objectsize)
, produced(0)
{
	initialize();
	begin();
}

ObjectGet::~ObjectGet() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
	if (outputp != 0) {
		delete outputp;
	}
}


void ObjectGet::initialize() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	std::memset(&context, 0, sizeof(context));
	context.hostName = hostname.c_str();
	context.bucketName = name.c_str();
	context.protocol = protocol;
	context.uriStyle = style;
	context.accessKeyId = id.c_str();
	context.secretAccessKey = secret.c_str();
	show(&context);
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

void ObjectGet::begin() {
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
	if (produced != EOF) {
		produced = output(buffer, bufferSize, bufferSize);
		if ((produced == EOF) && (outputp != 0)) {
			delete outputp;
			outputp = 0;
		}
	}
	return (produced != EOF) ? ::S3StatusOK : ::S3StatusAbortedByCallback;
}

void ObjectGet::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("ObjectGet@%p: complete\n", this);
}

}
}
}
}
