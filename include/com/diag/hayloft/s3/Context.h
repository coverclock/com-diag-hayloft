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

class Context {

public:

	explicit Context(
		const Session & session = Session(),
		const Credentials & credentials = Credentials(),
		const LocationConstraint & constraint = LocationConstraint(),
		const Protocol & protocol = Protocol(),
		const UniversalResourceIdentifierStyle & style = UniversalResourceIdentifierStyle()
	)
	: sessionp(&session)
	, credentialsp(&credentials)
	, constraintp(&constraint)
	, protocolp(&protocol)
	, stylep(&style)
	{}

	virtual ~Context() {}

private:

	const Session * sessionp;
	const Credentials * credentialsp;
	const LocationConstraint * constraintp;
	const Protocol * protocolp;
	const UniversalResourceIdentifierStyle * stylep;

public:

	const Session & getSession() const { return *sessionp; }
	const Credentials & getCredentials() const { return *credentialsp; }
	const LocationConstraint & getLocationConstraint() const { return *constraintp; }
	const Protocol & getProtocol() const { return *protocolp; }
	const UniversalResourceIdentifierStyle & getUniversalResourceIdentifierStyle() const { return *stylep; }

};

}
}
}
}

#endif
