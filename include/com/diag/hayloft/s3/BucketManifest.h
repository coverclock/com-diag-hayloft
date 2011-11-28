/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETMANIFEST_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETMANIFEST_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <map>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Selection.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketManifest : public Bucket {

public:

	typedef int64_t Epochalseconds;

	typedef uint64_t Octets;

	struct Entry {

		Entry(
			Epochalseconds lastModified = 0,
			const char * eTag = 0,
			Octets objectsize = 0,
			const char * ownerId = 0,
			const char * ownerDisplayName = 0
		);

		Epochalseconds modified;

		std::string etag;

		Octets size;

		std::string id;

		std::string display;

	};

	typedef std::map<std::string, Entry> Manifest;

	typedef std::pair<std::string, Entry> Pair;

private:

	static ::S3Status listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string prefix;

	std::string marker;

	std::string nextmarker;

	std::string delimiter;

	int maximum;

	bool truncated;

	Manifest manifest;

	::S3ListBucketHandler handler;

public:

	explicit BucketManifest(
		const char * bucketname,
		const Selection & selection = Selection(),
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit BucketManifest(
		const char * bucketname,
		Multiplex & multiplex,
		const Selection & selection = Selection(),
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~BucketManifest();

	virtual void start();

	const Manifest & getManifest() const { return manifest; }

	virtual const Entry * find(const char * name) const;

	virtual void reset();

protected:

	virtual ::S3Status entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes);

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
