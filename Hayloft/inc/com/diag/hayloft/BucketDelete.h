/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_BUCKETDELETE_
#define _H_COM_DIAG_HAYLOFT_BUCKETDELETE_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Bucket.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * BucketDelete is a Bucket Action which deletes a bucket.
 */
class BucketDelete : public Bucket {

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
	explicit BucketDelete(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name. A copy
	 *        is made of this C string.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 */
	explicit BucketDelete(
		const char * bucketname,
		const Plex & plex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 */
	explicit BucketDelete(
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit BucketDelete(
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Dtor. If this Action is in the BUSY state this forces the execution of
	 * all Actions on the same Plex and blocks until they all complete.
	 */
	virtual ~BucketDelete();

	/**
	 * Start this Action if it is not busy or forced.
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
