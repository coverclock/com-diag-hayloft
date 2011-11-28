/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketManifest::Entry::Entry(Epochalseconds lastModified, const char * eTag, Octets objectsize, const char * ownerId, const char * ownerDisplayName)
: modified(lastModified)
, etag((eTag != 0) ? eTag : "")
, size(objectsize)
, id((ownerId != 0) ? ownerId : "")
, display((ownerDisplayName != 0) ? ownerDisplayName : "")
{}

::S3Status BucketManifest::listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData) {
	BucketManifest * that = static_cast<BucketManifest*>(callbackData);
	::S3Status status = that->entry(isTruncated, nextMarker, contentsCount, contents, commonPrefixesCount, commonPrefixes);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	// See listBucketCallback() in s3.c from libs3 for Bryan's comments on how
	// S3 doesn't return nextMarker when there is no delimiter. I've modeled
	// this after his implementation which alternatively derives nextMarker.
	that->truncated = (isTruncated != 0);
	if ((nextMarker != 0) && (nextMarker[0] != '\0') && (contentsCount > 0) && (contents != 0)) {
		nextMarker = contents[contentsCount - 1].key;
	}
	if (nextMarker != 0) {
		that->nextmarker = nextMarker;
	}
	if (contents != 0) {
		for (int ii = 0; ii < contentsCount; ++ii) {
			const char * effective = (contents[ii].key != 0) ? contents[ii].key : "";
			Entry entry(contents[ii].lastModified, contents[ii].eTag, contents[ii].size, contents[ii].ownerId, contents[ii].ownerDisplayName);
			Logger::instance().log(level, "BucketManifest@%p: key=\"%s\" lastModified=%lld eTag=\"%s\" size=%llu ownerId=\"%s\" ownerDisplayName=\"%s\"\n", that, effective, entry.modified, entry.etag.c_str(), entry.size, entry.id.c_str(), entry.display.c_str());
			if ((status == ::S3StatusOK) && (that->manifest.size() < that->maximum)) {
				that->manifest.insert(Pair(effective, entry));
			}
		}
	}
	if (commonPrefixes != 0) {
		for (int ii = 0; ii < commonPrefixesCount; ++ii) {
			const char * effective = (commonPrefixes[ii] != 0) ? commonPrefixes[ii] : "";
			Logger::instance().log(level, "BucketManifest@%p: commonPrefix=\"%s\"\n", that, effective);
		}
	}
	Logger::instance().log(level, "BucketManifest@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	return status;
}

void BucketManifest::responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
	BucketManifest * that = static_cast<BucketManifest*>(callbackData);
	if ((status == ::S3StatusOK) && that->truncated && (that->maximum > that->manifest.size())) {
		that->truncated = false;
		that->execute();
	} else {
		(*that->Bucket::handler.completeCallback)(status, errorDetails, callbackData);
	}
}

BucketManifest::BucketManifest(const char * bucketname, const Selection & selection, const Context & context, const Session & session)
: Bucket(bucketname, context, session)
, prefix(selection.getPrefix())
, marker(selection.getMarker())
, nextmarker(selection.getMarker())
, delimiter(selection.getDelimiter())
, maximum(selection.getMaximum())
, truncated(false)
{
	initialize();
	execute();
}

BucketManifest::BucketManifest(const char * bucketname, Multiplex & multiplex, const Selection & selection, const Context & context, const Session & session)
: Bucket(bucketname, multiplex, context, session)
, prefix(selection.getPrefix())
, marker(selection.getMarker())
, nextmarker(selection.getMarker())
, delimiter(selection.getDelimiter())
, maximum(selection.getMaximum())
, truncated(false)
{
	initialize();
}

BucketManifest::~BucketManifest() {
	if ((state() == BUSY) && (requests != 0)) {
		(void)S3_runall_request_context(requests);
	}
}

void BucketManifest::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Bucket::handler.propertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;
	handler.listBucketCallback = &listBucketCallback;
}

void BucketManifest::execute() {
	Manifest::size_type size = manifest.size();
	if (maximum > size) {
		status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
		Logger::instance().debug("BucketManifest@%p: %s\n", this, (size == 0) ? "begin" : "continue");
		::S3_list_bucket(
			&context,
			prefix.empty() ? 0 : prefix.c_str(),
			nextmarker.empty() ? 0 : nextmarker.c_str(),
			delimiter.empty() ? 0 : delimiter.c_str(),
			maximum - size,
			requests,
			&handler,
			this
		);
	}
}

void BucketManifest::start() {
	if (state() != BUSY) {
		execute();
	}
}

const BucketManifest::Entry * BucketManifest::find(const char * name) const {
	const Entry * entry = 0;
	Manifest::const_iterator here = manifest.find(name);
	if (here != manifest.end()) {
		entry = &(here->second);
	}
	return entry;
}

void BucketManifest::reset() {
	if ((state() != BUSY)) {
		manifest.clear();
		nextmarker = marker;
		truncated = false;
	}
}

::S3Status BucketManifest::entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes) {
	return ::S3StatusOK;
}

}
}
}
}
