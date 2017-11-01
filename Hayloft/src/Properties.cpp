/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Properties.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

Properties::Properties(const char * contentType, const char * md5, const char * cacheControl, const char * contentDispositionFilename, const char * contentEncoding, const Epochalseconds expires, const Access & cannedAcl)
: type(::com::diag::grandote::set(contentType))
, checksum(::com::diag::grandote::set(md5))
, control(::com::diag::grandote::set(cacheControl))
, filename(::com::diag::grandote::set(contentDispositionFilename))
, encoding(::com::diag::grandote::set(contentEncoding))
, expiration(expires)
, access(cannedAcl.getAccess())
{}

const char * Properties::find(const char * key) const {
	const char * value = 0;
	Metadata::const_iterator here = metadata.find(key);
	if (here != metadata.end()) {
		value = (here->second).c_str();
	}
	return value;
}

}
}
}
