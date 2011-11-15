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
#include "com/diag/hayloft/s3/Region.h"
#include "com/diag/hayloft/s3/Protocol.h"
#include "com/diag/hayloft/s3/Style.h"
#include "com/diag/hayloft/s3/Access.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Context {

private:

	std::string id;

	std::string secret;

	std::string region;

	::S3Protocol protocol;

	::S3UriStyle style;

	::S3CannedAcl access;

public:

	explicit Context(
		const Credentials & cr = Credentials(),
		const Region & re = Region(),
		const Protocol & pr = Protocol(),
		const Style & st = Style(),
		const Access & ac = Access()
	);

	virtual ~Context() {}

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

	const char * getRegion() const { return region.c_str(); }

	const size_t getLength() const { return region.length(); }

	::S3Protocol getProtocol() const { return protocol; }

	::S3UriStyle getStyle() const { return style; }

	::S3CannedAcl getAccess() const { return access; }

	Context & setCredentials(const Credentials & cr = Credentials()) { id = cr.getId(); secret = cr.getSecret(); return *this; }

	Context & setRegion(const Region & re = Region()) { region = re.getRegion(); return *this; }

	Context & setProtocol(const Protocol & pr = Protocol()) { protocol = pr.getProtocol(); return *this; }

	Context & setStyle(const Style & st = Style()) { style = st.getStyle(); return *this; }

	Context & setAccess(const Access & ac = Access()) { access = ac.getAccess(); return *this; }

};

}
}
}
}

#endif
