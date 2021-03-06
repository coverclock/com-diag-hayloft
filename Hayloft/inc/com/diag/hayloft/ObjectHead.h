/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_OBJECTHEAD_
#define _H_COM_DIAG_HAYLOFT_OBJECTHEAD_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

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
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectHead(
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
	explicit ObjectHead(
		const Object & object
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit ObjectHead(
		const Object & object,
		const Plex & plex
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectHead();

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
