/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/s3/LifeCycle.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketManifest::Entry::Entry(const char * objectname, Epochalseconds lastModified, const char * eTag, Octets objectsize, const char * ownerId, const char * ownerDisplayName)
: key(objectname)
, modified(lastModified)
, etag(set(eTag))
, size(objectsize)
, owner(set(ownerId))
, display(set(ownerDisplayName))
{}

Status BucketManifest::listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData) {
	BucketManifest * that = static_cast<BucketManifest*>(callbackData);
	Status status = that->entry(isTruncated, nextMarker, contentsCount, contents, commonPrefixesCount, commonPrefixes);
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
			if ((contents[ii].key != 0) && (contents[ii].key[0] != '\0')) {
				Entry entry(contents[ii].key, contents[ii].lastModified, contents[ii].eTag, contents[ii].size, contents[ii].ownerId, contents[ii].ownerDisplayName);
				Logger::instance().log(level, "BucketManifest@%p: key=\"%s\" lastModified=%lld eTag=\"%s\" size=%llu ownerId=\"%s\" ownerDisplayName=\"%s\"\n", that, entry.getKey(), entry.getModified(), entry.getETag(), entry.getSize(), entry.getOwnerId(), entry.getOwnerDisplayName());
				if ((status == ::S3StatusOK) && (that->manifest.size() < that->maximum)) {
					that->manifest.insert(Pair(contents[ii].key, entry));
				}
			}
		}
	}
	if (commonPrefixes != 0) {
		for (int ii = 0; ii < commonPrefixesCount; ++ii) {
			if ((commonPrefixes[ii] != 0) && (*commonPrefixes[ii] != '\0')) {
				Logger::instance().log(level, "BucketManifest@%p: commonPrefix=\"%s\"\n", that, commonPrefixes[ii]);
				if (status == ::S3StatusOK) {
					that->common.push_back(commonPrefixes[ii]);
				}
			}
		}
	}
	Logger::instance().log(level, "BucketManifest@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	return status;
}

void BucketManifest::responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData) {
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

BucketManifest::BucketManifest(const char * bucketname, const Plex & plex, const Selection & selection, const Context & context, const Session & session)
: Bucket(bucketname, plex, context, session)
, prefix(selection.getPrefix())
, marker(selection.getMarker())
, nextmarker(selection.getMarker())
, delimiter(selection.getDelimiter())
, maximum(selection.getMaximum())
, truncated(false)
{
	initialize();
}

BucketManifest::BucketManifest(const Bucket & bucket, const Selection & selection)
: Bucket(bucket)
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

BucketManifest::BucketManifest(const Bucket & bucket, const Plex & plex, const Selection & selection)
: Bucket(bucket, plex)
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
	if ((state() == BUSY) && (pending != 0)) {
		(void)S3_runall_request_context(pending);
	}
}

void BucketManifest::initialize() {
	status = static_cast<Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Bucket@%p: prefix=\"%s\"\n", this, prefix.c_str());
		logger.debug("Bucket@%p: marker=\"%s\"\n", this, nextmarker.c_str());
		logger.debug("Bucket@%p: delimiter=\"%s\"\n", this, delimiter.c_str());
		logger.debug("Bucket@%p: maximum=%d\n", this, maximum);
	}
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Bucket::handler.propertiesCallback;
	handler.responseHandler.completeCallback = &responseCompleteCallback;
	handler.listBucketCallback = &listBucketCallback;
}

void BucketManifest::execute() {
	Manifest::size_type size = manifest.size();
	if (maximum > size) {
		status = static_cast<Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
		Logger::instance().debug("BucketManifest@%p: %s\n", this, (size == 0) ? "begin" : "continue");
		LifeCycle::instance().start(*this);
		::S3_list_bucket(
			&context,
			set(prefix),
			set(nextmarker),
			set(delimiter),
			maximum - size,
			pending,
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
		common.clear();
		nextmarker = marker;
		truncated = false;
	}
}

Status BucketManifest::entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes) {
	Logger::instance().debug("BucketManifest%p: entry\n", this);
	return ::S3StatusOK;
}

}
}
}
}
