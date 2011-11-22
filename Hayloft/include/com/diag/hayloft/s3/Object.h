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
#include "com/diag/desperado/MemoryBarrier.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;
class Multiplex;

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

	std::string key;

	std::string id;

	std::string secret;

	std::string hostname;

	std::string name;

	::S3Protocol protocol;

	::S3UriStyle style;

	::S3RequestContext * requests;

	::S3Status status;

	::S3BucketContext context;

	::S3ResponseHandler handler;

	::S3Status state() const { ::com::diag::desperado::MemoryBarrier barrier; return status; }

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

	operator bool() const { ::S3Status temporary = state(); return ((temporary != IDLE) && (temporary != BUSY)); }

	bool isIdle() const { return (state() == IDLE); }

	bool isBusy() const { return (state() == BUSY); }

	bool isInaccessible() const { return (state() == ::S3StatusHttpErrorForbidden); }

	bool isNonexistent() const { return (state() == ::S3StatusHttpErrorNotFound); }

	bool isRetryable() const { return (::S3_status_is_retryable(state()) != 0); }

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

	const char * getHostName() const { return hostname.c_str(); }

	const char * getName() const { return name.c_str(); }

	const char * getKey() const { return key.c_str(); }

	::S3Protocol getProtocol() const { return protocol; }

	::S3UriStyle getStyle() const { return style; }

	::S3RequestContext * getRequests() const { return requests; }

	::S3Status getStatus(const char ** description = 0) const;

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
