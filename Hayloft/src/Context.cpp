/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Context.h"

namespace com {
namespace diag {
namespace hayloft {

Context::Context(
	const Credentials & cr,
	const Region & re,
	const Protocol & pr,
	const Style & st,
	const Access & ac
)
: id(cr.getId())
, secret(cr.getSecret())
, region(re.getRegion())
, protocol(pr.getProtocol())
, style(st.getStyle())
, access(ac.getAccess())
{}

}
}
}
