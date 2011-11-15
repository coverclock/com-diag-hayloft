/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ACCESS
#define _H_COM_DIAG_HAYLOFT_S3_ACCESS

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Access {

private:

	::S3CannedAcl access;

public:

	static const ::S3CannedAcl DEFAULT = ::S3CannedAclPrivate;

	explicit Access(::S3CannedAcl cannedAcl = DEFAULT);

	virtual ~Access() {}

	::S3CannedAcl getAccess() const { return access; }

	Access & setAccess(::S3CannedAcl cannedAcl = DEFAULT) { access = cannedAcl; return *this; }

};

class AccessPrivate : public Access {

public:

	explicit AccessPrivate()
	: Access(::S3CannedAclPrivate)
	{}

	virtual ~AccessPrivate() {}

};

class AccessPublicRead : public Access {

public:

	explicit AccessPublicRead()
	: Access(::S3CannedAclPublicRead)
	{}

	virtual ~AccessPublicRead() {}

};

class AccessPublicReadWrite : public Access {

public:

	explicit AccessPublicReadWrite()
	: Access(::S3CannedAclPublicReadWrite)
	{}

	virtual ~AccessPublicReadWrite() {}

};

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
