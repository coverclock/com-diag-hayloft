/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ObjectList.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ObjectList::Entry::Entry(Epochalseconds lastModified, const char * eTag, Octets objectsize, const char * ownerId, const char * ownerDisplayName)
: modified(lastModified)
, etag((eTag != 0) ? eTag : "")
, size(objectsize)
, id((ownerId != 0) ? ownerId : "")
, display((ownerDisplayName != 0) ? ownerDisplayName : "")
{}

::S3Status ObjectList::listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData) {
	ObjectList * that = static_cast<ObjectList*>(callbackData);
	::S3Status status = that->entry(isTruncated, nextMarker, contentsCount, contents, commonPrefixesCount, commonPrefixes);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	if (contents != 0) {
		for (int ii = 0; ii < contentsCount; ++ii) {
			const char * effective = (contents[ii].key != 0) ? contents[ii].key : "";
			Entry entry(contents[ii].lastModified, contents[ii].eTag, contents[ii].size, contents[ii].ownerId, contents[ii].ownerDisplayName);
			Logger::instance().log(level, "ObjectList@%p: key=\"%s\" lastModified=%%lld eTag=\"%s\" size=%llu ownerId=\"%s\" ownerDisplayName=\"%s\"\n", that, effective, entry.modified, entry.etag.c_str(), entry.size, entry.id.c_str(), entry.display.c_str());
			if (status == ::S3StatusOK) {
				that->list.insert(Pair(effective, entry));
			}
		}
	}
	if (commonPrefixes != 0) {
		for (int ii = 0; ii < commonPrefixesCount; ++ii) {
			const char * effective = (commonPrefixes[ii] != 0) ? commonPrefixes[ii] : "";
			Logger::instance().log(level, "ObjectList@%p: commonPrefix=\"%s\"\n", that, effective);
		}
	}
	Logger::instance().log(level, "ObjectList@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	return status;
}

ObjectList::ObjectList(const char * bucketname, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
{
	initialize();
	execute();
}

ObjectList::ObjectList(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: Bucket(bucketname, multiplex, context, session)
{
	initialize();
}

ObjectList::~ObjectList() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void ObjectList::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Bucket::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Bucket::handler.completeCallback;
	handler.listBucketCallback = &listBucketCallback;
}

void ObjectList::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ObjectList@%p: begin\n", this);
	::S3_list_bucket(
		&context,
		0,
		0,
		0,
		intmaxof(int),
		requests,
		&handler,
		this
	);
}

void ObjectList::start() {
	if (state() != BUSY) {
		execute();
	}
}

::S3Status ObjectList::entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes) {
	return ::S3StatusOK;
}

}
}
}
}
