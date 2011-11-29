/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETHEAD_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETHEAD_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Region.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * BucketHead is a Bucket Action which retrieves the meta-data associated with
 * a bucket, which is its existence and its region (location constraint).
 */
class BucketHead : public Bucket {

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	char constraint[Region::LENGTH];

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 *        This reference is only used during construction.
	 */
	explicit BucketHead(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 */
	explicit BucketHead(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Dtor. If the Action is in the BUSY state this forces the execution of
	 * all Actions on the same Multiplex and blocks until they all complete.
	 */
	virtual ~BucketHead();

	/**
	 * Start the Action if it is IDLE, or re-start it if it is neither IDLE nor
	 * BUSY.
	 */
	virtual void start();

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
