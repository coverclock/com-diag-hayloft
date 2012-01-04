/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_LOGSET_
#define _H_COM_DIAG_HAYLOFT_S3_LOGSET_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Log.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * LogSet establishes a relationship between two S3 buckets, one that is used to
 * store S3 objects, the other used to store log files automatically generated
 * by S3 to record the actions on the first bucket.
 */
class LogSet : public Log {

protected:

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	int count;

	::S3AclGrant * grants;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 * @param log refers to the bucket into which the logs are to be written.
	 *        This reference is only used during construction.
	 * @param keyprefix is an optional C string that is a key (object name)
	 *        prefix used for any logs generated.
	 * @param grant refers to a Grant whose access control list will be applied
	 *        to any of the logs generated. This reference is only used during
	 *        construction.
	 */
	explicit LogSet(
		const Bucket & bucket,
		const Bucket & log,
		const char * keyprefix = 0,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param log refers to the bucket into which the logs are to be written.
	 *        This reference is only used during construction.
	 * @param keyprefix is an optional C string that is a key (object name)
	 *        prefix used for any logs generated.
	 * @param grant refers to a Grant whose access control list will be applied
	 *        to any of the logs generated. This reference is only used during
	 *        construction.
	 */
	explicit LogSet(
		const Bucket & bucket,
		const Plex & plex,
		const Bucket & log,
		const char * keyprefix = 0,
		const Grant & grant = Grant()
	);

	/**
	 * Dtor.
	 */
	virtual ~LogSet();

	/**
	 * Start the Action if it is not busy or forced.
	 *
	 * @param force if true cause the start to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool start(bool force = false);

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
