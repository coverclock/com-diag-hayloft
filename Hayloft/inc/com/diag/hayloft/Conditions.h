/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_CONDITIONS_
#define _H_COM_DIAG_HAYLOFT_CONDITIONS_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/types.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Conditions are used by ObjectGet Actions to filter Objects that may be
 * gotten. Objects may be selected based on when they were modified or what
 * their eTag is. The default constructor arguments construct a Condition that
 * accepts all, filters none.
 */
class Conditions {

protected:

	Epochalseconds since;

	Epochalseconds notsince;

	std::string match;

	std::string notmatch;

public:

	/**
	 * Ctor.
	 *
	 * @param ifModifiedSince selects on the condition that the Object has been
	 *        modified since a date and time specified in seconds since the
	 *        UNIX Epoch. -1 indicates this condition is not used.
	 * @param ifNotModifiedSince selects on the condition that the Object has
	 *        not been modified since a date and time specified in seconds since
	 *        the UNIX Epoch. -1 indicates that this condition is not used.
	 * @param ifMatchETag selects on the condition that the Object has a
	 *        matching eTag. Null or an empty C string indicates that this
	 *        condition is not used.
	 * @param ifNotMatchETag selects on the condition that the Object does not
	 *        have a matching eTag. Null or an empty C string indicates that
	 *        this condition is not used.
	 */
	explicit Conditions(
		Epochalseconds ifModifiedSince = -1,
		Epochalseconds ifNotModifiedSince = -1,
		const char * ifMatchETag = 0,
		const char * ifNotMatchETag = 0
	);

	/**
	 * Dtor.
	 */
	virtual ~Conditions() {}

	/**
	 * Get the ifModifiedSince condition.
	 *
	 * @return the ifModifiedSince condition.
	 */
	Epochalseconds getSince() const { return since; }

	/**
	 * Get the ifNotModifiedSince condition.
	 *
	 * @return the ifNotModifiedSince condition.
	 */
	Epochalseconds getNotSince() const { return notsince; }

	/**
	 * Get the ifMatchETag condition.
	 *
	 * @return the ifMatchETag condition.
	 */
	const char * getMatch() const { return match.c_str(); }

	/**
	 * Get the ifNotMatchETag condition.
	 *
	 * @return the ifNotMatchETag condition.
	 */
	const char * getNotMatch() const { return notmatch.c_str(); }

	/**
	 * Set the ifModifiedsince condition.
	 *
	 * @param ifModifiedSince selects on the condition that the Object has been
	 *        modified since a date and time specified in seconds since the
	 *        UNIX Epoch. -1 indicates this condition is not used.
	 * @return a reerence to this object.
	 */
	Conditions & setSince(Epochalseconds ifModifiedSince = -1) { since = ifModifiedSince; return *this; }

	/**
	 * Set the ifNotModifiedSince condition.
	 *
	 * @param ifNotModifiedSince selects on the condition that the Object has
	 *        not been modified since a date and time specified in seconds since
	 *        the UNIX Epoch. -1 indicates that this condition is not used.
	 * @return a reference to this object.
	 */
	Conditions & setNotSince(Epochalseconds ifNotModifiedSince = -1) { notsince = ifNotModifiedSince; return *this; }

	/**
	 * Set the ifMatchETag condition.
	 *
	 * @param ifMatchETag selects on the condition that the Object has a
	 *        matching eTag. Null or an empty C string indicates that this
	 *        condition is not used.
	 * @return a reference to this object.
	 */
	Conditions & setMatch(const char * ifMatchETag = 0) { match = ::com::diag::grandote::set(ifMatchETag); return *this; }

	/**
	 * Set the ifNotMatchETag condition.
	 *
	 * @param ifNotMatchETag selects on the condition that the Object does not
	 *        have a matching eTag. Null or an empty C string indicates that
	 *        this condition is not used.
	 * @return a reference to this object.
	 */
	Conditions & setNotMatch(const char * ifNotMatchETag = 0) { notmatch = ::com::diag::grandote::set(ifNotMatchETag); return *this; }

};

}
}
}

#endif
