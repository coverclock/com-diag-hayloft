/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ACCESS_
#define _H_COM_DIAG_HAYLOFT_S3_ACCESS_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Access represents one of any of the various predefined access control lists
 * provided by libs3. You can use this base class to create your own Access
 * object with any of the ::S3CannedAcl values, or use one of the derived
 * classes.
 */
class Access {

protected:

	::S3CannedAcl access;

public:

	/**
	 * This defines the default libs3 ::S3CannedAcl value if no value is
	 * specified during construction. (You should change this is it isn't
	 * what you want.)
	 */
	static const ::S3CannedAcl DEFAULT = ::S3CannedAclPrivate;

	/**
	 * Ctor.
	 *
	 * @param cannedAcl is the libs3 ::S3CannedAcl value this object will
	 *        represent.
	 */
	explicit Access(::S3CannedAcl cannedAcl = DEFAULT);

	/**
	 * Dtor.
	 */
	virtual ~Access() {}

	/**
	 * Get the libs3 ::S3CannedAcl value of this object.
	 *
	 * @return the libs3 ::S3CannedAcl value of this object.
	 */
	::S3CannedAcl getAccess() const { return access; }

	/**
	 * Set the libs3 ::S3CannedAcl value of this object.
	 *
	 * @param cannedAcl is the libs3 ::S3CannedAcl value this object will
	 *        represent.
	 * @return a reference to this object.
	 */
	Access & setAccess(::S3CannedAcl cannedAcl = DEFAULT) { access = cannedAcl; return *this; }

};

/**
 * AccessPrivate is an Access that represents the libs3 ::S3CannedAclPrivate
 * ACL.
 */
class AccessPrivate : public Access {

public:

	explicit AccessPrivate()
	: Access(::S3CannedAclPrivate)
	{}

	virtual ~AccessPrivate() {}

};

/**
 * AccessPublicRead is an Access that represents the libs3
 * ::S3CannedAclPublicRead ACL.
 */
class AccessPublicRead : public Access {

public:

	explicit AccessPublicRead()
	: Access(::S3CannedAclPublicRead)
	{}

	virtual ~AccessPublicRead() {}

};

/**
 * AccessPublicReadWrite is an Access that represents the libs3
 * ::S3CannedAclPublicReadWrite ACL.
 */
class AccessPublicReadWrite : public Access {

public:

	explicit AccessPublicReadWrite()
	: Access(::S3CannedAclPublicReadWrite)
	{}

	virtual ~AccessPublicReadWrite() {}

};

/**
 * AccessAuthenticatedReadWrite is an Access that represents the libs3
 * ::S3CannedAclAuthenticatedReadWrite ACL.
 */
class AccessAuthenticatedRead : public Access {

public:

	explicit AccessAuthenticatedRead()
	: Access(::S3CannedAclAuthenticatedRead)
	{}

	virtual ~AccessAuthenticatedRead() {}

};

}
}
}
}

#endif
