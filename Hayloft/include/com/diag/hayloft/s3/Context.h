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

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Credentials;
class LocationConstraint;
class Protocol;
class UniversalResourceIdentifierStyle;
class CannedAccessControlList;

class Context {

public:

	static const Context context;

private:

	const Credentials * credentialsp;
	const LocationConstraint * constraintp;
	const Protocol * protocolp;
	const UniversalResourceIdentifierStyle * stylep;
	const CannedAccessControlList * listp;

public:

	explicit Context(
		const Credentials & credentials = context.getCredentials(),
		const LocationConstraint & constraint = context.getLocationConstraint(),
		const Protocol & protocol = context.getProtocol(),
		const UniversalResourceIdentifierStyle & style = context.getUniversalResourceIdentifierStyle(),
		const CannedAccessControlList & list = context.getCannedAccessControlList()
	)
	: credentialsp(&credentials)
	, constraintp(&constraint)
	, protocolp(&protocol)
	, stylep(&style)
	, listp(&list)
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
