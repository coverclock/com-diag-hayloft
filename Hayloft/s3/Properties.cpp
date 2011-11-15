/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Properties.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Properties::Properties(const char * contentType, const char * md5, const char * cacheControl, const char * contentDispositionFilename, const char * contentEncoding, const int64_t expires, const Access & cannedAcl)
: type(set(contentType, 0, ""))
, checksum(set(md5, 0, ""))
, control(set(cacheControl, 0, ""))
, filename(set(contentDispositionFilename, 0, ""))
, encoding(set(contentEncoding, 0, ""))
, expiration(expires)
, access(cannedAcl.getAccess())
{
}

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
