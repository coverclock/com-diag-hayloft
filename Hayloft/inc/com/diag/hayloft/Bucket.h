/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_BUCKET_
#define _H_COM_DIAG_HAYLOFT_BUCKET_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/Container.h"
#include "com/diag/hayloft/Session.h"
#include "com/diag/hayloft/Context.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Bucket is a Container which describes an S3 bucket which is identified by
 * a non-canonical name provided by the application, and has associated with it
 * a region (a.k.a. location constraint) and a libs3 canned access control list.
 * Practically speaking, an S3 bucket is a web site hosted in S3 that is
 * identified by a internet domain name derived from the canonical bucket name
 * and the Amazon Web Services (AWS) Endpoint name which contains S3 objects
 * which can be accessed via HTTP or HTTPS using Universal Resource Locations
 * or URLs.
 */
class Bucket : public Container {

protected:

	std::string name;

	std::string region;

	::S3CannedAcl access;

	std::string * temporary;

public:

	/**
	 * Ctor. Use this for don't care.
	 */
	explicit Bucket();

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name. A copy
	 *        is made of this C string.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 *        This reference is only used during construction.
	 */
	explicit Bucket(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name. A copy
	 *        is made of this C string.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 */
	explicit Bucket(
		const char * bucketname,
		const Plex & plex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 */
	explicit Bucket(
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Bucket(
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~Bucket();

	/**
	 * Get the non-canonical (application) bucket name associated with this
	 * Bucket.
	 *
	 * @return the non-canonical (application) bucket name.
	 */
	const char * getName() const { return name.c_str(); }

	/**
	 * Get the region (location constraint) associated with this Bucket.
	 *
	 * @return the region (location constraint).
	 */
	const char * getRegion() const { return region.c_str(); }

	/**
	 * Get the libs3 ::S3CannedAcl value associated with this Bucket.
	 *
	 * @return the libs3 ::S3CannedAcl value.
	 */
	::S3CannedAcl getAccess() const { return access; }

	/**
	 * Return true if libs3 thinks the canonical bucket name is valid,
	 * false otherwise.
	 *
	 * @return true if the canonical bucket name is valid.
	 */
	bool isValid() const;

private:

	void initialize();

};

}
}
}

#endif
