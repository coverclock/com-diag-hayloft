/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * BucketValid validates a bucket name with the libs3 bucket name validation
 * function. BucketValid is not really an Action. It executes synchronously and
 * on the local system, even though it extends the Bucket class. It indicates
 * the bucket name is valid by return a successful status.
 */
class BucketValid : public Bucket {

public:

	/**
	 * Ctor. There is only a synchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 *        This reference is only used during construction.
	 */
	explicit BucketValid(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Dtor.
	 */
	virtual ~BucketValid() {}

	/**
	 * Since this pseudo-Action is never in the IDLE or BUSY states, always
	 * return true.
	 *
	 * @return true.
	 */
	operator bool() const { return true; }

private:

	void initialize();

	void execute();

};


}
}
}
}

#endif
