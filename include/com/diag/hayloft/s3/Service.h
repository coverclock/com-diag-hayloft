/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SERVICE_
#define _H_COM_DIAG_HAYLOFT_S3_SERVICE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Action.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Service is an Action which is accessed by using an access key ID (like a
 * login or account number) provided by AWS, a secret access key (like a
 * password or encryption key) also provided by AWS, an end point (essentially
 * a hostname), and an HTTP protocol (Secure or Unsecure). It is a superclass
 * that is not used directly.
 */
class Service : public Action {

protected:

	std::string id;

	std::string secret;

	std::string endpoint;

	::S3Protocol protocol;

public:

	/**
	 * Ctor. This is used by the synchronous interface.
	 *
	 * @param accessKeyId is the access key ID (like a login or account number)
	 *        provided by AWS used to access this Container.
	 * @param secretAccessKey is the secret access key (like a password or
	 *        encryption key) provided by AWS used to access this Container.
	 * @param endPoint is the end point (hostname) used to access this
	 *        Container.
	 * @param httpProtocol is the HTTP protocol, Secure or Unsecure, used to
	 *        access this Container.
	 */
	explicit Service(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		::S3Protocol httpProtocol
	);

	/**
	 * Ctor. This is used by the asynchronous interface.
	 *
	 * @param accessKeyId is the access key ID (like a login or account number)
	 *        provided by AWS used to access this Container.
	 * @param secretAccessKey is the secret access key (like a password or
	 *        encryption key) provided by AWS used to access this Container.
	 * @param endPoint is the end point (hostname) used to access this
	 *        Container.
	 * @param httpProtocol is the HTTP protocol, Secure or Unsecure, used to
	 *        access this Container.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Service(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		::S3Protocol httpProtocol,
		Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~Service();

	/**
	 * Return the twenty-character access key ID.
	 *
	 * @return the twenty-character access key ID.
	 */
	const char * getId() const { return id.c_str(); }

	/**
	 * Return the forty-character secret access key.
	 *
	 * @return the forty-character secret access key.
	 */
	const char * getSecret() const { return secret.c_str(); }

	/**
	 * Return the end point name.
	 *
	 * @return the end point name.
	 */
	const char * getEndpoint() const { return endpoint.c_str(); }

	/**
	 * Return the protocol.
	 *
	 * @return the protocol.
	 */
	::S3Protocol getProtocol() const { return protocol; }

private:

	void initialize();

};

}
}
}
}

#endif
