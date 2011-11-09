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
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Context {

private:

	std::string id;
	std::string secret;
	std::string constraint;
	::S3Protocol protocol;
	::S3UriStyle style;
	::S3CannedAcl list;

public:

	explicit Context(
		const Credentials & cr = Credentials(),
		const LocationConstraint & co = LocationConstraint(),
		const Protocol & pr = Protocol(),
		const UniversalResourceIdentifierStyle & st = UniversalResourceIdentifierStyle(),
		const CannedAccessControlList & li = CannedAccessControlList()
	);

	virtual ~Context() {}

	const char * getId() const { return id.c_str(); }
	const char * getSecret() const { return secret.c_str(); }
	const char * getLocationConstraint() const { return constraint.c_str(); }
	::S3Protocol getProtocol() const { return protocol; }
	::S3UriStyle getStyle() const { return style; }
	::S3CannedAcl getCannedAccessControlList() const { return list; }

};

}
}
}
}

#endif
