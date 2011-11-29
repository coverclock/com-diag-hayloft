/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CONDITIONS_
#define _H_COM_DIAG_HAYLOFT_S3_CONDITIONS_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Conditions {

protected:

	Epochalseconds since;

	Epochalseconds notsince;

	std::string match;

	std::string notmatch;

public:

	explicit Conditions(
		Epochalseconds ifModifiedSince = -1,
		Epochalseconds ifNotModifiedSince = -1,
		const char * ifMatchETag = 0,
		const char * ifNotMatchETag = 0
	);

	virtual ~Conditions() {}

	Epochalseconds getSince() const { return since; }

	Epochalseconds getNotSince() const { return notsince; }

	const char * getMatch() const { return match.c_str(); }

	const char * getNotMatch() const { return notmatch.c_str(); }

	Conditions & setSince(Epochalseconds ifModifiedSince = -1) { since = ifModifiedSince; return *this; }

	Conditions & setNotSince(Epochalseconds ifNotModifiedSince = -1) { notsince = ifNotModifiedSince; return *this; }

	Conditions & setMatch(const char * ifMatchETag = 0) { match = set(ifMatchETag, 0, ""); return *this; }

	Conditions & setNotMatch(const char * ifNotMatchETag = 0) { notmatch = set(ifNotMatchETag, 0, ""); return *this; }

};

}
}
}
}

#endif
