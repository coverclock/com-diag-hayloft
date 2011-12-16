/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_GRANTSET_
#define _H_COM_DIAG_HAYLOFT_S3_GRANTSET_

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
 * GrantSet establishes a new access control list for an S3 bucket or object.
 */
class GrantSet : public Grant {

private:

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	int count;

	::S3AclGrant * grants;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit GrantSet(
		const Bucket & bucket,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit GrantSet(
		const Object & object,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit GrantSet(
		const Bucket & bucket,
		const Plex & plex,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit GrantSet(
		const Object & object,
		const Plex & plex,
		const Grant & grant = Grant()
	);

	/**
	 * Dtor.
	 */
	virtual ~GrantSet();

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
