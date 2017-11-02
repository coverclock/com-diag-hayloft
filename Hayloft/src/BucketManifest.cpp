/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/BucketManifest.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/generics.h"

namespace com {
namespace diag {
namespace hayloft {

BucketManifest::Entry::Entry(const char * objectname, Epochalseconds lastModified, const char * eTag, Octets objectsize, const char * ownerId, const char * ownerDisplayName)
: key(objectname)
, modified(lastModified)
, etag(::com::diag::grandote::set(eTag))
, size(objectsize)
, owner(::com::diag::grandote::set(ownerId))
, display(::com::diag::grandote::set(ownerDisplayName))
{}

Status BucketManifest::listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData) {
	BucketManifest * that = static_cast<BucketManifest*>(callbackData);
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	Status status = that->entry(isTruncated, nextMarker, contentsCount, contents, commonPrefixesCount, commonPrefixes);
	::com::diag::grandote::MaskableLogger::Level level = (status == ::S3StatusOK) ? ::com::diag::grandote::MaskableLogger::DEBUG : ::com::diag::grandote::MaskableLogger::NOTICE;
	logger.log(level, "BucketManifest@%p: isTruncated=%d nextMarker=\"%s\" contentsCount=%d commonPrefixesCount=%d\n", that, isTruncated, (nextMarker != 0) ? nextMarker : "(nil)", contentsCount, commonPrefixesCount);
	that->truncated = (isTruncated != 0);
	if (contents != 0) {
		for (int ii = 0; ii < contentsCount; ++ii) {
			if ((contents[ii].key != 0) && (contents[ii].key[0] != '\0')) {
				Entry entry(contents[ii].key, contents[ii].lastModified, contents[ii].eTag, contents[ii].size, contents[ii].ownerId, contents[ii].ownerDisplayName);
				logger.log(level, "BucketManifest@%p: key=\"%s\" lastModified=%lld eTag=\"%s\" size=%llu ownerId=\"%s\" ownerDisplayName=\"%s\"\n", that, entry.getKey(), entry.getModified(), entry.getETag(), entry.getSize(), entry.getOwnerId(), entry.getOwnerDisplayName());
				if ((status == ::S3StatusOK) && (that->manifest.size() < that->maximum)) {
					that->manifest.insert(Pair(contents[ii].key, entry));
				}
			}
		}
	}
	if (commonPrefixes != 0) {
		for (int ii = 0; ii < commonPrefixesCount; ++ii) {
			if ((commonPrefixes[ii] != 0) && (*commonPrefixes[ii] != '\0')) {
				logger.log(level, "BucketManifest@%p: commonPrefix=\"%s\"\n", that, commonPrefixes[ii]);
				if (status == ::S3StatusOK) {
					that->common.push_back(commonPrefixes[ii]);
				}
			}
		}
	}
	// See listBucketCallback() in s3.c from libs3 for Bryan's comments on how
	// S3 doesn't return nextMarker when there is no delimiter. I've modeled
	// this after his implementation which alternatively derives nextMarker.
	if ((contents != 0) && (contentsCount > 0)) {
		that->nextmarker = contents[contentsCount - 1].key;
		logger.log(level, "BucketManifest@%p: nextMarker=\"%s\"\n", that, that->nextmarker.c_str());
	}
	logger.log(level, "BucketManifest@%p: status=%d=\"%s\"\n", that, status, tostring(status));
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
}

void BucketManifest::initialize() {
	state(static_cast<Status>(IDLE));
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
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
		state(static_cast<Status>(BUSY));
		::com::diag::grandote::MaskableLogger::instance().debug("BucketManifest@%p: %s\n", this, (size == 0) ? "begin" : "continue");
		Bucket::execute();
		::S3_list_bucket(
			&context,
			::com::diag::grandote::set(prefix),
			::com::diag::grandote::set(nextmarker),
			::com::diag::grandote::set(delimiter),
			maximum - size,
			handle,
			&handler,
			this
		);
	}
}

bool BucketManifest::start() {
	if (Bucket::start()) {
		execute();
		return true;
	} else {
		return false;
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

bool BucketManifest::reset() {
	if (Bucket::reset()) {
		manifest.clear();
		common.clear();
		nextmarker = marker;
		truncated = false;
		return true;
	} else {
		return false;
	}
}

Status BucketManifest::entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes) {
	::com::diag::grandote::MaskableLogger::instance().debug("BucketManifest%p: entry\n", this);
	return ::S3StatusOK;
}

}
}
}
