/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKET_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Status.h"
#include "com/diag/desperado/target.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session;
class Multiplex;

class Bucket {

public:

	/**
	 * For virtual host style URIs, AWS S3 places a limit of this many
	 * characters on the bucket name.
	 */
	static const size_t LENGTH = 64;

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string id;

	std::string secret;

	std::string hostname;

	std::string name;

	std::string region;

	::S3Protocol protocol;

	::S3UriStyle style;

	::S3CannedAcl access;

	::S3RequestContext * requests;

	::S3Status status;

	::S3ResponseHandler handler;

public:

	explicit Bucket(
		const Session & session,
		const char * bucketname,
		const Context & context = Context()
	);

	explicit Bucket(
		const Session & session,
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context()
	);

	virtual ~Bucket();

	operator bool() const { return (status != BUSY); }

	bool isBusy() const { return (status == BUSY); }

	bool isRetryable() const { return (::S3_status_is_retryable(status) != 0); }

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

	const char * getHostName() const { return hostname.c_str(); }

	const char * getName() const { return name.c_str(); }

	const char * getRegion() const { return region.c_str(); }

	::S3Protocol getProtocol() const { return protocol; }

	::S3UriStyle getStyle() const { return style; }

	::S3CannedAcl getAccess() const { return access; }

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
