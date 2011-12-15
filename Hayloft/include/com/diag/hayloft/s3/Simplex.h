/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SIMPLEX_
#define _H_COM_DIAG_HAYLOFT_S3_SIMPLEX_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Plex.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Simplex is a Plex that uses the default constructor. This results in a
 * Plex that causes an Action to be synchronous (because the S3RequestContext
 * pointer that Simplex provides is NULL) but which is not started automatically
 * during construction (because it uses the constructor that takes a Plex
 * reference).
 */
class Simplex : public Plex {

public:

	/**
	 * Ctor.
	 */
	explicit Simplex() {}

	/**
	 * Dtor.
	 */
	virtual ~Simplex() {}

};

}
}
}
}

#endif