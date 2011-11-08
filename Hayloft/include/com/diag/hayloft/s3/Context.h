/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CONTEXT
#define _H_COM_DIAG_HAYLOFT_S3_CONTEXT

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/s3/CannedAccessControlList.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Context {

private:

	static const Credentials credentials;
	static const LocationConstraint constraint;
	static const Protocol protocol;
	static const UniversalResourceIdentifierStyle style;
	static const CannedAccessControlList list;

	const Credentials * credentialsp;
	const LocationConstraint * constraintp;
	const Protocol * protocolp;
	const UniversalResourceIdentifierStyle * stylep;
	const CannedAccessControlList * listp;

public:

	explicit Context(
		const Credentials & rc = credentials,
		const LocationConstraint & rl = constraint,
		const Protocol & rp = protocol,
		const UniversalResourceIdentifierStyle & ru = style,
		const CannedAccessControlList & ra = list
	)
	: credentialsp(&rc)
	, constraintp(&rl)
	, protocolp(&rp)
	, stylep(&ru)
	, listp(&ra)
	{}

	virtual ~Context() {}

	const Credentials & getCredentials() const { return *credentialsp; }
	const LocationConstraint & getLocationConstraint() const { return *constraintp; }
	const Protocol & getProtocol() const { return *protocolp; }
	const UniversalResourceIdentifierStyle & getUniversalResourceIdentifierStyle() const { return *stylep; }
	const CannedAccessControlList & getCannedAccessControlList() const { return *listp; }

};

}
}
}
}

#endif
