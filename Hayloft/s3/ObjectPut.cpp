/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {


ObjectPut::ObjectPut(const Bucket & bucket, const char * keyname, Size totalsize, ::com::diag::desperado::Input & source, const Properties & props)
: Object(bucket, keyname)
, type(props.getType())
, checksum(props.getChecksum())
, control(props.getControl())
, filename(props.getFilename())
, encoding(props.getEncoding())
, size(totalsize)
, input(source)
{
	initialize(props);
}

ObjectPut::~ObjectPut() {
	if (requests != 0) {
		(void)S3_runall_request_context(requests);
	}
}


void ObjectPut::initialize(const Properties & props) {
	Logger & logger = Logger::instance();
	logger.debug("ObjectPut@%p: begin\n", this);
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	context.hostName = hostname.c_str();
	context.bucketName = name.c_str();
	context.protocol = protocol;
	context.uriStyle = style;
	context.accessKeyId = id.c_str();
	context.secretAccessKey = secret.c_str();
	show(&context, Logger::DEBUG);
	properties.contentType = type.empty() ? 0 : type.c_str();
	properties.md5 = checksum.empty() ? 0 : checksum.c_str();
	properties.cacheControl = control.empty() ? 0 : control.c_str();
	properties.contentDispositionFilename = filename.empty() ? 0 : filename.c_str();
	properties.contentEncoding = encoding.empty() ? 0 : encoding.c_str();
	properties.expires = props.getExpires();
	properties.cannedAcl = props.getAccess();
	const Properties::Metadata & md = props.getMetadata();
	properties.metaDataCount = md.size();
	if (properties.metaDataCount > 0) {
		Properties::Metadata::const_iterator here;
		Properties::Metadata::const_iterator end;
		for (here = md.begin(), end = md.end(); here != end; ++here) {
			metadata.insert(Properties::Pair(here->first, here->second));
		}
		::S3NameValue * pair = new ::S3NameValue [properties.metaDataCount];
		properties.metaData = pair;
		for (here = metadata.begin(), end = metadata.end(); here != end; ++here, ++pair) {
			pair->name = (here->first).c_str();
			pair->value = (here->second).c_str();
		}
	} else {
		properties.metaData = 0;
	}
	show(&properties, Logger::DEBUG);
	handler.responseHandler.propertiesCallback = &responsePropertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;
	handler.putObjectDataCallback = &putObjectDataCallback;
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
	ssize_t rc = input(buffer, 1, bufferSize);
	return (rc == EOF) ? 0 : rc;
}

void ObjectPut::complete(::S3Status s3status, const ::S3ErrorDetails * errorDetails) {
	Logger::instance().debug("ObjectPut@%p: end\n", this);
	Object::complete(s3status, errorDetails);
}

}
}
}
}
