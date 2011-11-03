/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKET
#define _H_COM_DIAG_HAYLOFT_S3_BUCKET

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket {

public:

	explicit Bucket(const char * bucketName);

	virtual ~Bucket();

	virtual bool valid(
		Session & session,
		const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle()
	);

	virtual void test(
		Session & session,
		const Credentials & credentials = Credentials(),
		const Protocol & protocol = Protocol(),
		const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle()
	);

	virtual void create(
		Session & session,
		const Credentials & credentials = Credentials(),
		const LocationConstraint & constraint = LocationConstraint(),
		const Protocol & protocol = Protocol(),
		const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle(),
		const CannedAccessControlList & acl = CannedAccessControlList()
	);

	virtual void remove(
		Session & session,
		const Credentials & credentials = Credentials(),
		const LocationConstraint & constraint = LocationConstraint(),
		const Protocol & protocol = Protocol(),
		const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle()
	);

private:

	std::string name;

public:

	const char * getBucketName() const { return name.c_str(); }

};

}
}
}
}

#endif
