/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CANNEDACCESSCONTROLLIST
#define _H_COM_DIAG_HAYLOFT_S3_CANNEDACCESSCONTROLLIST

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/Logger.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class CannedAccessControlList {

public:

	explicit CannedAccessControlList(::S3CannedAcl cannedacl = ::S3CannedAclPrivate)
	: canned(cannedacl)
	{
		Logger::instance().debug("CannedAccessControlList@%p: cannedacl=%d\n", this, canned);
	}

	virtual ~CannedAccessControlList() {}

	::S3CannedAcl getCannedAccessControlList() const { return canned; }

private:

	::S3CannedAcl canned;

};

class CannedAccessControlListPrivate : public CannedAccessControlList {

public:

	explicit CannedAccessControlListPrivate()
	: CannedAccessControlList(::S3CannedAclPrivate)
	{}

	virtual ~CannedAccessControlListPrivate() {}

};

class CannedAccessControlListPublicRead : public CannedAccessControlList {

public:

	explicit CannedAccessControlListPublicRead()
	: CannedAccessControlList(::S3CannedAclPublicRead)
	{}

	virtual ~CannedAccessControlListPublicRead() {}

};

class CannedAccessControlListPublicReadWrite : public CannedAccessControlList {

public:

	explicit CannedAccessControlListPublicReadWrite()
	: CannedAccessControlList(::S3CannedAclPublicReadWrite)
	{}

	virtual ~CannedAccessControlListPublicReadWrite() {}

};

class CannedAccessControlListAuthenticatedRead : public CannedAccessControlList {

public:

	explicit CannedAccessControlListAuthenticatedRead()
	: CannedAccessControlList(::S3CannedAclAuthenticatedRead)
	{}

	virtual ~CannedAccessControlListAuthenticatedRead() {}

};

}
}
}
}

#endif
