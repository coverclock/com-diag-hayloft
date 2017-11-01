/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CONTAINER_
#define _H_COM_DIAG_HAYLOFT_S3_CONTAINER_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/Service.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Container is a Service that is identified with a canonical name and has a
 * Uniform Resource Indentifier (URI) style, Virtual Host or Path, with which
 * it is accessed. Is is a superclass of other classes like Bucket and Object
 * and is not used directly.
 */
class Container : public Service {

protected:

	std::string canonical;

	::S3UriStyle style;

	::S3BucketContext context;

public:

	/**
	 * Ctor. This is used by the synchronous interface.
	 *
	 * @param accessKeyId is the access key ID (like a login or account number)
	 *        provided by AWS used to access this Container. A copy is made of
	 *        this C string.
	 * @param secretAccessKey is the secret access key (like a password or
	 *        encryption key) provided by AWS used to access this Container. A
	 *        copy is made of this C string.
	 * @param endPoint is the end point (hostname) used to access this
	 *        Container. A copy is made of this C string.
	 * @param canonicalBucketName is the canonical bucket name used to access
	 *        this Container. A copy is made of this C string.
	 * @param httpProtocol is the HTTP protocol, Secure or Unsecure, used to
	 *        access this Container.
	 * @param uriStyle is the URI style, Virtual Host or Path, used to access
	 *        this Container.
	 */
	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * canonicalBucketName,
		::S3Protocol httpProtocol,
		::S3UriStyle uriStyle
	);

	/**
	 * Ctor. This is used by the asynchronous interface.
	 *
	 * @param accessKeyId is the access key ID (like a login or account number)
	 *        provided by AWS used to access this Container. A copy is made of
	 *        this C string.
	 * @param secretAccessKey is the secret access key (like a password or
	 *        encryption key) provided by AWS used to access this Container. A
	 *        copy is made of this C string.
	 * @param endPoint is the end point (hostname) used to access this
	 *        Container. A copy is made of this C string.
	 * @param canonicalBucketName is the canonical bucket name used to access
	 *        this Container. A copy is made of this C string.
	 * @param httpProtocol is the HTTP protocol, Secure or Unsecure, used to
	 *        access this Container.
	 * @param uriStyle is the URI style, Virtual Host or Path, used to access
	 *        this Container.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * canonicalBucketName,
		::S3Protocol httpProtocol,
		::S3UriStyle uriStyle,
		 const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~Container();

	/**
	 * Get the canonical name associated with this Container.
	 *
	 * @return the canonical name.
	 */
	const char * getCanonical() const { return canonical.c_str(); }

	/**
	 * Get the URI style associated with this Container.
	 *
	 * @return the URI style.
	 */
	::S3UriStyle getStyle() const { return style; }

private:

	void initialize();

	/*
	 * Copy constructor and assignment operator POISONED in a superclass.
	 */

};

}
}
}
}

#endif
