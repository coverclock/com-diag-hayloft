/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SESSION_
#define _H_COM_DIAG_HAYLOFT_S3_SESSION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 *
 * Canonicalizing the bucket name by appending the USER AGENT string to it
 * separated by a dot (providing the USER AGENT string is not empty) is simply
 * my convention for Hayloft. Using a domain name as part of the bucket name
 * makes it more likely to be unique. I think it's a useful convention. Mapping
 * the entire bucket name to all lower cases seems necessary (and is recommended
 * by the DevGuide). Otherwise you get weird messages from BucketTest about keys
 * not matching when using VIRTUAL HOST style URIs, and BucketCreate will tell
 * you that the bucket name is invalid.
 */

#include <string>
#include "com/diag/hayloft/s3/Endpoint.h"
#include "com/diag/desperado/target.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session {

protected:

    /**
     * Points to the default instance of this object.
     */
    static Session * singleton;

	std::string bucketsuffix;

	std::string useragent;

	std::string endpoint;

	::S3Status status;

public:

	static const char * BUCKET_SUFFIX_STR() { return ""; }

	static const char * BUCKET_SUFFIX_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_BUCKETSUFFIX"; }

	static const char * USER_AGENT_STR() { return ""; }

	static const char * USER_AGENT_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_USERAGENT"; }

	/**
	 * Allocates a reference to a new object of this type suitably initialized
	 * with default parameters.
	 * @return a reference to a new object of this type.
	 */
	static Session & factory();

	/**
	 * Establishes a new default instance of an object of this type.
	 * @param that refers to an object of this type.
	 * @return that.
	 */
	static Session & instance(Session & that);

    /**
     * Returns a reference to the default instance of an object of this type.
     * @return a reference to the default instance of an object of this type.
     */
	static Session & instance();

	explicit Session(
		const char * bucketSuffix = 0,
		const char * userAgentInfo = 0,
		const Endpoint & endPoint = Endpoint(),
		int flags = S3_INIT_ALL
	);

	virtual ~Session();

	operator bool() const { return (status == ::S3StatusOK); }

	::S3Status getStatus() const { return status; }

	const char * getBucketSuffix() const { return bucketsuffix.c_str(); }

	const char * getUserAgent() const { return useragent.c_str(); }

	const char * getEndpoint() const { return endpoint.c_str(); }

	virtual const char * canonicalize(std::string & canonical) const;

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	Session(const Session& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	Session& operator=(const Session& that);

};

}
}
}
}

#endif
