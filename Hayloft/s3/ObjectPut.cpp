/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectPut.h"
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

int ObjectPut::putObjectDataCallback(int bufferSize, char * buffer, void * callbackData) {
	ObjectPut * that = static_cast<ObjectPut*>(callbackData);
	int rc = that->put(bufferSize, buffer);
	Logger::Level level = (rc >= 0) ? Logger::DEBUG : Logger::NOTICE;
	Logger::instance().log(level, "ObjectPut@%p: requested=%d returned=%d\n", that, bufferSize, rc);
	return (rc < 0) ? 0 : rc;
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
{
	initialize(props.getMetadata());
	execute();
}
ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, Multiplex & multiplex, Input & source, Octets objectsize, const Properties & props)
: Object(keyname, bucket, multiplex)
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
{
	initialize(props.getMetadata());
}

ObjectPut::ObjectPut(const char * keyname, const Bucket & bucket, Multiplex & multiplex, Input * sourcep, Octets objectsize, const Properties & props)
: Object(keyname, bucket, multiplex)
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
{
	initialize(props.getMetadata());
}

ObjectPut::~ObjectPut() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
	if (taken != 0) {
		delete taken;
	}
	if (properties.metaData != 0) {
		delete [] properties.metaData;
	}
}

void ObjectPut::initialize(const Properties::Metadata & settings) {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
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
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Object::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Object::handler.completeCallback;
	handler.putObjectDataCallback = &putObjectDataCallback;
}

void ObjectPut::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectPut@%p: begin\n", this);
	::S3_put_object(
		&context,
		key.c_str(),
		size,
		&properties,
		requests,
		&handler,
		this
	);
}

int ObjectPut::put(int bufferSize, char * buffer) {
	ssize_t consumed = 0;
	if (input != 0) {
		consumed = (*input)(buffer, 1, bufferSize);
		if (consumed == EOF) {
			consumed = 0;
			input = 0;
		}
	}
	return consumed;
}

void ObjectPut::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("ObjectPut@%p: end\n", this);
}

}
}
}
}
