/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_GRANTGET_
#define _H_COM_DIAG_HAYLOFT_S3_GRANTGET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Grant.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * GrantGet retrieves the access control list for an S3 bucket or object.
 */
class GrantGet : public Grant {

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	char ownerid[OWNER_LEN];

	char ownerdisplayname[DISPLAY_LEN];

	int count;

	::S3AclGrant * grants;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit GrantGet(
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit GrantGet(
		const Object & object
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit GrantGet(
		const Bucket & bucket,
		const Multiplex & multiplex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit GrantGet(
		const Object & object,
		const Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~GrantGet();

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
