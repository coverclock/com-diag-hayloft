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

#include <map>
#include <string>
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/s3/Container.h"

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
	 * ObjectHead and ObjectGet return metadata in the form of (keyword,value)
	 * tuples. ObjectPut and ObjectCopy apply metadata in the same form to S3
	 * objects. The only Object Action which does not use metadata is Object
	 * Delete. This is a map containing those tuples.
	 */
	typedef std::map<std::string, std::string> Metadata;

	/**
	 * ObjectHead and ObjectGet return metadata in the form of (keyword,value)
	 * tuples. ObjectPut and ObjectCopy apply metadata in the same form to S3
	 * objects. The only Object Action which does not use metadata is Object
	 * Delete. This is a tuple.
	 */
	typedef std::pair<std::string, std::string> Pair;

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * responseProperties, void * callbackData);

protected:

	std::string key;

	std::string type;

	std::string etag;

	std::string authentic;

	Octets length;

	Epochalseconds modified;

	Metadata metadata;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for don't care.
	 *
	 */
	explicit Object();

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
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
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Object(
		const char * keyname,
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 */
	explicit Object(
		const Object & object
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Object(
		const Object & object,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~Object();

	/**
	 * Get the key (object name) associated with this Object.
	 *
	 * @return the key (object name).
	 */
	const char * getKey() const { return key.c_str(); }

	/**
	 * Get the content type provided by S3 once this Action completes.
	 * @return the content type.
	 */
	const char * getContentType() const { return type.c_str(); }

	/**
	 * Get the eTag provided by S3 once this Action completes.
	 * @return the eTag.
	 */
	const char * getETag() const { return etag.c_str(); }

	/**
	 * Get the content length provided by S3 once this Action completes.
	 * @return the length.
	 */
	Octets getContentLength() const { return length; }

	/**
	 * Get the modification time provided by S3 once this Action completes.
	 * @return the modification time.
	 */
	Epochalseconds getModificationTime() const { return modified; }

	/**
	 * Get a constant reference to the Metadata map.
	 *
	 * @return a constant reference to the Metadata map.
	 */
	const Metadata & getMetadata() const { return metadata; }

	/**
	 * If there is a metadata entry with the given key, return its value,
	 * otherwise return null.
	 *
	 * @param key is the metadata keyword.
	 * @return a metadata value for the key or null.
	 */
	const char * find(const char * key) const;

	/**
	 * Generate an authenticated GET query string that can be used by a web
	 * browser until the specified expiration date has passed.
	 *
	 * @param expires is the expiration date in seconds since the UNIX Epoch or
	 *        -1 for the longest possible expiration date.
	 * @param resource is a C string that identifies the sub-resource (e.g.
	 *        libs3 offers the example "?torrent") or NULL for none.
	 * @return an authenticated query string or NULL if an error occurred.
	 */
	const char * authenticated(Epochalseconds expires = -1, const char * resource = 0);

private:

	void initialize();

};

}
}
}
}

#endif
