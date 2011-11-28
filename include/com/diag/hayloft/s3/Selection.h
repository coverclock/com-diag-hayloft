/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SELECTION_
#define _H_COM_DIAG_HAYLOFT_S3_SELECTION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Selection {

protected:

	std::string prefix;

	std::string marker;

	std::string delimiter;

	int maximum;

public:

	explicit Selection(
		const char * namePrefix = 0,
		const char * nextMarker = 0,
		const char * nameDelimiter = 0,
		int maxkeys = intmaxof(int)
	);

	virtual ~Selection() {}

	const char * getPrefix() const { return prefix.c_str(); }

	const char * getMarker() const { return marker.c_str(); }

	const char * getDelimiter() const { return delimiter.c_str(); }

	int getMaximum() const { return maximum; }

	Selection & setPrefix(const char * namePrefix = 0) { prefix = set(namePrefix, 0, ""); return *this; }

	Selection & setMarker(const char * nextMarker = 0) { marker = set(nextMarker, 0, ""); return *this; }

	Selection & setDelimiter(const char * nameDelimiter = 0) { delimiter = set(nameDelimiter, 0, ""); return *this; }

	Selection & setMaximum(int maxkeys = intmaxof(int)) { maximum = maxkeys; return *this; }

};

}
}
}
}

#endif
