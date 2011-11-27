/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTLIST_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTLIST_

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

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class ObjectList : public Bucket {

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

	typedef std::map<std::string, Entry> List;

	typedef std::pair<std::string, Entry> Pair;

private:

	static ::S3Status listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData);

protected:

	List list;

	::S3ListBucketHandler handler;

public:

	explicit ObjectList(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit ObjectList(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~ObjectList();

	virtual void start();

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
