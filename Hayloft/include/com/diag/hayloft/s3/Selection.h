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

/**
 * Selection allows a selection critera to be specified when doing a
 * BucketManifest to list all of the S3 objects in an S3 bucket. This is
 * useful for buckets that may be quite large. See BucketManifest.
 */
class Selection {

protected:

	std::string prefix;

	std::string marker;

	std::string delimiter;

	int maximum;

public:

	/**
	 * Ctor.
	 *
	 * @param namePrefix causes only objects whose keys are prefixed by this
	 *        C string to be returned. A copy is made of this C string. A null
	 *        or empty value means this selection criteria isn't used.
	 * @param nextMarker causes only objects whose keys are after this C string
	 *        in collating sequence to be returned. A copy is made of this C
	 *        string. A null or empty value means this selection criteria isn't
	 *        used.
	 * @param nameDelimiter causes those objects whose keys have substrings that
	 *        are prefixed by namePrefix and delimited by this C string to be
	 *        returned, not in the Manifest map but in the Common list. A copy
	 *        is made of this C string. A null or empty value means this
	 *        selection criteria isn't used.
	 * @param maxkeys is the maximum number of keys to return in the Manifest
	 *        map.
	 */
	explicit Selection(
		const char * namePrefix = 0,
		const char * nextMarker = 0,
		const char * nameDelimiter = 0,
		int maxkeys = intmaxof(int)
	);

	/**
	 * Dtor.
	 */
	virtual ~Selection() {}

	/**
	 * Get the namePrefix.
	 *
	 * @return the namePrefix.
	 */
	const char * getPrefix() const { return prefix.c_str(); }

	/**
	 * Get the nextMarker.
	 *
	 * @return the nextMarker.
	 */
	const char * getMarker() const { return marker.c_str(); }

	/**
	 * Get the nameDelimiter.
	 *
	 * @return the nameDelimiter.
	 */
	const char * getDelimiter() const { return delimiter.c_str(); }

	/**
	 * Get the maxkeys.
	 *
	 * @return the maxkeys.
	 */
	int getMaximum() const { return maximum; }

	/**
	 * Set the namePrefix.
	 *
	 * @param namePrefix causes only objects whose keys are prefixed by this
	 *        C string to be returned. A copy is made of this C string. A null
	 *        or empty value means this selection criteria isn't used.
	 * @return a reference to this object.
	 */
	Selection & setPrefix(const char * namePrefix = 0) { prefix = set(namePrefix); return *this; }

	/**
	 * Set the nextMarker.
	 *
	 * @param nextMarker causes only objects whose keys are after this C string
	 *        in collating sequence to be returned. A copy is made of this C
	 *        string. A null or empty value means this selection criteria isn't
	 *        used.
	 * @return a reference to this object.
	 */
	Selection & setMarker(const char * nextMarker = 0) { marker = set(nextMarker); return *this; }

	/**
	 * Set the nameDelimiter.
	 *
	 * @param nameDelimiter causes those objects whose keys have substrings that
	 *        are prefixed by namePrefix and delimited by this C string to be
	 *        returned, not in the Manifest map but in the Common list. A copy
	 *        is made of this C string. A null or empty value means this
	 *        selection criteria isn't used.
	 * @return a reference to this object.
	 */
	Selection & setDelimiter(const char * nameDelimiter = 0) { delimiter = set(nameDelimiter); return *this; }

	/**
	 * Set the maxkeys.
	 *
	 * @param maxkeys is the maximum number of keys to return in the Manifest
	 *        map.
	 * @return a reference to this object.
	 */
	Selection & setMaximum(int maxkeys = intmaxof(int)) { maximum = maxkeys; return *this; }

};

}
}
}
}

#endif
