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

#include "com/diag/hayloft/s3/Status.h"
#include "com/diag/desperado/target.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class Object {

public:

	/**
	 * AWS S3 places a limit of this many characters on the object name (a.k.a.
	 * key).
	 */
	static const size_t LENGTH = 1024;

	typedef uint64_t Octets;

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string hostname;

	std::string name;

	::S3Protocol protocol;

	::S3UriStyle style;

	std::string id;

	std::string secret;

	::S3RequestContext * requests;

	std::string key;

	::S3Status status;

	::S3ResponseHandler handler;

public:

	explicit Object(
		const Bucket & bucket,
		const char * keyname
	);

	virtual ~Object();

	operator bool() const { return (status != BUSY); }

	bool isBusy() const { return (status == BUSY); }

	bool isRetryable() const { return (::S3_status_is_retryable(status) != 0); }

	::S3Status getStatus() const { return status; }

	const char * getKey() const { return key.c_str(); }

protected:

	virtual ::S3Status properties(const ::S3ResponseProperties * properties);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

};

}
}
}
}

#endif
