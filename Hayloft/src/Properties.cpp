/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Properties.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Properties::Properties(const char * contentType, const char * md5, const char * cacheControl, const char * contentDispositionFilename, const char * contentEncoding, const Epochalseconds expires, const Access & cannedAcl)
: type(set(contentType))
, checksum(set(md5))
, control(set(cacheControl))
, filename(set(contentDispositionFilename))
, encoding(set(contentEncoding))
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
}
