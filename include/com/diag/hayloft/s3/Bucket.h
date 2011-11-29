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

#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/s3/Container.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

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

public:

	/**
	 * For virtual host style URIs, AWS S3 places a limit of this many
	 * characters on the bucket name. The limit on path style URIs is
	 * more like 255 and can be found as S3_MAX_BUCKET_NAME_SIZE.
	 */
	static const size_t LENGTH = 64;

protected:

	std::string name;

	std::string region;

	::S3CannedAcl access;

	std::string * temporary;

public:

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
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 */
	explicit Bucket(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
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

private:

	void initialize();

};

}
}
}
}

#endif
