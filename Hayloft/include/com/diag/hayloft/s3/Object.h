/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECT_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Container.h"
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class Object : public Container {

public:

	/**
	 * AWS S3 places a limit of this many characters on the object name (a.k.a.
	 * key).
	 */
	static const size_t LENGTH = 1024;

	typedef uint64_t Octets;

protected:

	std::string key;

public:

	explicit Object(
		const char * keyname,
		const Bucket & bucket
	);

	explicit Object(
		const char * keyname,
		const Bucket & bucket,
 		Multiplex & multiplex
	);

	virtual ~Object();

	bool isInaccessible() const { ::S3Status temporary = state(); return ((temporary == ::S3StatusHttpErrorForbidden) || (temporary == ::S3StatusErrorAccessDenied)); }

	bool isNonexistent() const { ::S3Status temporary = state(); return ((temporary == ::S3StatusHttpErrorNotFound) || (temporary == ::S3StatusErrorNoSuchKey) || (temporary == ::S3StatusErrorNoSuchBucket)); }

	const char * getKey() const { return key.c_str(); }

};

}
}
}
}

#endif
