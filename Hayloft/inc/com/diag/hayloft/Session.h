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
 */

#include <string>
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/Endpoint.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Session initializes libs3 during construction and denitializes it during
 * destruction. Session contains parameters that required during initialization,
 * or which are specific to a particular application or user. These parameters
 * include the a strings that is concatenated to all S3 bucket names during a
 * process called canonicalization, a user agent that identifies a specific
 * application to S3 and to AWS, and an end point name. The default Session
 * is one lazily constructed as a singleton upon request and derives these
 * parameters from the environment. However the application can replace this
 * Session with its own and with its own explicitly specified parameters.
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
class Session {

public:

	/**
	 * This is the name of the environmental variable with which you can
	 * specify the bucket suffix. I use one based on my domain name, e.g.
	 * ".foo.bar.com". NOTE THE LEADING DOT.
	 */
	static const char * BUCKET_SUFFIX_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_BUCKETSUFFIX"; }

	/**
	 * This is the name of the environmental variable with which you can
	 * specify the user agent name.
	 */
	static const char * USER_AGENT_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_USERAGENT"; }

	/**
	 * This is the bucket suffix used of none is specified in the environment.
	 * You can change this to something specific to your application.
	 */
	static const char * BUCKET_SUFFIX_STR() { return ""; }

	/**
	 * This is the user agent used of none is specified in the environment.
	 * You can change this to something specific to your application.
	 */
	static const char * USER_AGENT_STR() { return ""; }

	/**
	 * Allocates a reference to a new object of this type suitably initialized
	 * with default parameters.
	 *
	 * @return a reference to a new object of this type.
	 */
	static Session & factory();

	/**
	 * Establishes a new default instance of an object of this type.
	 *
	 * @param that refers to an object of this type.
	 * @return that.
	 */
	static Session & instance(Session & that);

    /**
     * Returns a reference to the default instance of an object of this type.
     *
     * @return a reference to the default instance of an object of this type.
     */
	static Session & instance();

protected:

    /**
     * Points to the default instance of this object.
     */
    static Session * singleton;

	std::string bucketsuffix;

	std::string useragent;

	std::string endpoint;

	Status status;

public:

	/**
	 * Ctor.
	 *
	 * @param bucketSuffix is the bucket suffix. Although it is not required,
	 *        the suffix should probably have a leading "." if it is specified
	 *        at all. If null or empty, and if no bucket suffix is specified in
	 *        the environment, no bucket suffix is used. Note that bucket names
	 *        must be universally unique across all of S3. They can be thought
	 *        of as web site names. A copy is made of this C string.
	 * @param userAgentInfo is the user agent. If null or empty, and if no user
	 *        agent is specified in the environment, no user agent is passed to
	 *        S3. A copy is made of this C string.
	 * @param endPoint is the EndPoint to be used.
	 * @param flags is passed to libs3 but in practice has no meaning outside of
	 *        the Windows environment.
	 */
	explicit Session(
		const char * bucketSuffix = 0,
		const char * userAgentInfo = 0,
		const Endpoint & endPoint = Endpoint(),
		int flags = S3_INIT_ALL
	);

	/**
	 * Dtor.
	 */
	virtual ~Session();

	/**
	 * Returns true if construction was successful.
	 *
	 * @return true if construction was successful.
	 */
	operator bool() const { return (status == ::S3StatusOK); }

	/**
	 * Return the status resulting from the libs3 initialization.
	 *
	 * @return the status.
	 */
	Status getStatus() const { return status; }

	/**
	 * Get the bucket suffix.
	 *
	 * @return the bucket suffix.
	 */
	const char * getBucketSuffix() const { return bucketsuffix.c_str(); }

	/**
	 * Get the user agent.
	 *
	 * @return the user agent.
	 */
	const char * getUserAgent() const { return useragent.c_str(); }

	/**
	 * Get the end point name.
	 *
	 * @return the end point name.
	 */
	const char * getEndpoint() const { return endpoint.c_str(); }

	/**
	 * Canonicalize a name (typically an application bucket name) by appending
	 * the bucket suffix (if it exists) to the name and then converting all of
	 * the upper case alphas in the result to lower case. Derived classes may
	 * replace this function with one more to their liking.
	 *
	 * @param canonical is the as yet uncanonicalized application bucket name.
	 * @return a C string from the input string.
	 */
	virtual const char * canonicalize(std::string & canonical) const;

	/**
	 * Deanonicalize a name (typically an S3 bucket name) by stripping
	 * the bucket suffix (if it exists) from the name. Derived classes may
	 * replace this function with one more to their liking. Since the method
	 * has no way of knowing the capitalization of the original application
	 * bucket name, the result is still in all lower case alphas. This can be
	 * used to (try to) match application bucket names against the canonical
	 * bucket names returned by ServiceManifest.
	 *
	 * @param decanonical is the as yet canonicalized application bucket name.
	 * @return a C string from the input string.
	 */
	virtual const char * decanonicalize(std::string & decanonical) const;

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
