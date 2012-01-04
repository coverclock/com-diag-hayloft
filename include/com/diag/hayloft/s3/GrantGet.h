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
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * GrantGet retrieves the access control list for an S3 bucket or object.
 */
class GrantGet : public Grant {

protected:

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	char ownerid[S3_MAX_GRANTEE_USER_ID_SIZE + 1];

	char ownerdisplayname[S3_MAX_GRANTEE_DISPLAY_NAME_SIZE + 1];

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
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit GrantGet(
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit GrantGet(
		const Object & object,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~GrantGet();

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
