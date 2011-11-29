/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTDELETE_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTDELETE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;
class Multiplex;

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

	explicit ObjectDelete(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectDelete();

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
