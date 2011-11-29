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

/**
 * Object is a Container which describes an S3 object which is identified by
 * a key provided by the application. An Object is a Container by virtue of
 * the fact that it is always associated with a Bucket with a canonical name.
 * In a traditional file system sense, the Container is the directory path name
 * and the Object is the file. Practically speaking, an S3 object is a opaque
 * blob of application data, whose size is measured in integral octets, which
 * resides inside an S3 bucket, and which can be accessed via HTTP or HTTPS
 * using Universal Resource Locations or URLs.
 */
class Object : public Container {

public:

	/**
	 * AWS S3 places a limit of this many characters on the object name (a.k.a.
	 * the key).
	 */
	static const size_t LENGTH = S3_MAX_KEY_SIZE;

	/**
	 * S3 object size encoded in units of eight-bit bytes.
	 */
	typedef uint64_t Octets;

protected:

	std::string key;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit Object(
		const char * keyname,
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Object(
		const char * keyname,
		const Bucket & bucket,
 		Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~Object();

	/**
	 * Return the key (object name) associated with this Object.
	 * @return the key (object name).
	 */
	const char * getKey() const { return key.c_str(); }

};

}
}
}
}

#endif
