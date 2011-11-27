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

#include "com/diag/hayloft/s3/Container.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

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

	explicit Bucket(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit Bucket(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~Bucket();

	bool isInaccessible() const { return (state() == ::S3StatusErrorAccessDenied); }

	bool isNonexistent() const { return (state() == ::S3StatusErrorNoSuchBucket); }

	const char * getName() const { return name.c_str(); }

	const char * getRegion() const { return region.c_str(); }

	::S3CannedAcl getAccess() const { return access; }

private:

	void initialize();

};

}
}
}
}

#endif
