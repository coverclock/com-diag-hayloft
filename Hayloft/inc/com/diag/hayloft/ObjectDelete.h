/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTDELETE_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTDELETE_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;
class Plex;

/**
 * ObjectDelete is an Object Action which deletes an Object from a Bucket.
 */
class ObjectDelete : public Object {

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this
	 *        C string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit ObjectDelete(
		const char * keyname,
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this
	 *        C string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectDelete(
		const char * keyname,
		const Bucket & bucket,
		const Plex & plex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 */
	explicit ObjectDelete(
		const Object & object
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectDelete(
		const Object & object,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectDelete();

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
}

#endif
