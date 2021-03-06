/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_LOGGET_
#define _H_COM_DIAG_HAYLOFT_LOGGET_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "com/diag/hayloft/Log.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * LogGet retrieves a relationship (if such a relationship exists) between two
 * S3 buckets, one that is used to store S3 objects, the other used to store
 * log files automatically generated by S3 to record the actions on the first
 * bucket.
 */
class LogGet : public Log {

protected:

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	char targetbucket[S3_MAX_BUCKET_NAME_SIZE + 1];

	char targetprefix[S3_MAX_KEY_SIZE + 1];

	int count;

	::S3AclGrant * grants;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 */
	explicit LogGet(
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit LogGet(
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~LogGet();

	/**
	 * Start this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool start();

private:

	void initialize();

	void execute();

};

}
}
}

#endif
