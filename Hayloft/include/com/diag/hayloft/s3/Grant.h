/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_GRANT_
#define _H_COM_DIAG_HAYLOFT_S3_GRANT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/s3/Container.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;
class Object;

/**
 * Grant represents an S3 access control list (ACL) for an S3 bucket or object.
 * Grants can be retrieved from an existing bucket or object, imported
 * from an XML file in a pre-defined format, or created from scratch.
 */
class Grant : public Container {

public:

	/**
	 *
	 */
	static const size_t OWNER_LEN = S3_MAX_GRANTEE_USER_ID_SIZE;

	/**
	 *
	 */
	static const size_t DISPLAY_LEN = S3_MAX_GRANTEE_DISPLAY_NAME_SIZE;

	/**
	 *
	 */
	static const size_t COUNT = S3_MAX_ACL_GRANT_COUNT;

protected:

	std::string key;

	const char * keypointer;

	std::string owner;

	std::string display;

	int count;

	::S3AclGrant * grants;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 */
	explicit Grant();

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit Grant(
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit Grant(
		const Object & object,
		const Bucket & bucket
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param grant refers to another grant associated with this object. This
	 *        reference is only used during construction.
	 */
	explicit Grant(
		const Grant & grant
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
	explicit Grant(
		const Bucket & bucket,
		const Multiplex & multiplex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Object & object,
		const Bucket & bucket,
		const Multiplex & multiplex
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param grant refers to another grant associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Grant & grant,
		const Multiplex & multiplex
	);

	/**
	 * Dtor.
	 */
	virtual ~Grant();

	/**
	 * Get the key (object name) associated with this Object.
	 *
	 * @return the key (object name).
	 */
	const char * getKey() const { return key.c_str(); }

	/**
	 * Get the content type provided by S3 once this Action completes.
	 * @return the content type.
	 */
	const char * getOwnerId() const { return owner.c_str(); }

	/**
	 * Get the eTag provided by S3 once this Action completes.
	 * @return the eTag.
	 */
	const char * getOwnerDisplayName() const { return display.c_str(); }

private:

	void initialize(const Grant * that);

};

}
}
}
}

#endif
