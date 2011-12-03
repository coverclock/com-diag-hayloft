/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTHEAD_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTHEAD_

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

/**
 * ObjectHead is an Object Action which retrieves the meta-data for the Object
 * but not the Object contents. The meta-data for an Object consists of its
 * libs3 ::S3ResponseProperties. (All of the meta-data handling is actually
 * done in the base class since it is common to all Object Actions.)
 */
class ObjectHead : public Object {

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit ObjectHead(
		const char * keyname,
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectHead(
		const char * keyname,
		const Bucket & bucket,
		const Multiplex & multiplex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 */
	explicit ObjectHead(
		const Object & object
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectHead(
		const Object & object,
		const Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectHead();

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
