/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_LOGSET_
#define _H_COM_DIAG_HAYLOFT_S3_LOGSET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Log.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 *
 */
class LogSet : public Log {

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

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
	 * @param multiplex refers to the Multiplex responsible for executing this
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
		const Multiplex & multiplex,
		const Bucket & log,
		const char * keyprefix = 0,
		const Grant & grant = Grant()
	);

	/**
	 * Dtor.
	 */
	virtual ~LogSet();

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
