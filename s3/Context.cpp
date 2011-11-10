/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Context.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Context::Context(
	const Credentials & cr,
	const LocationConstraint & co,
	const Protocol & pr,
	const UniversalResourceIdentifierStyle & st,
	const CannedAccessControlList & li
)
: id(cr.getId())
, secret(cr.getSecret())
, constraint(co.getLocationConstraint())
, protocol(pr.getProtocol())
, style(st.getStyle())
, list(li.getCannedAccessControlList())
{
	Logger & logger = Logger::instance();
	logger.debug("Context@%p: id=\"%s\"[%zu]\n", this, Credentials::obfuscate(id.c_str()), id.length());
	logger.debug("Context@%p: secret=\"%s\"[%zu]\n", this, Credentials::obfuscate(secret.c_str()), secret.length());
	logger.debug("Context@%p: constraint=\"%s\"\n", this, constraint.c_str());
	logger.debug("Context@%p: protocol=%d\n", this, protocol);
	logger.debug("Context@%p: style=%d\n", this, style);
	logger.debug("Context@%p: list=%d\n", this, list);
}

}
}
}
}
