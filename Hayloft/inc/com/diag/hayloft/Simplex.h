/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_SIMPLEX_
#define _H_COM_DIAG_HAYLOFT_SIMPLEX_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Plex.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Simplex is one way in which synchronous Actions may be managed. Simplex is a
 * Plex that uses the default constructor. This results in a Plex that causes an
 * Action to be synchronous (because the S3RequestContext pointer that Simplex
 * provides is NULL) but which is not started automatically during construction
 * (because it uses the constructor that takes a Plex reference). Hence, Simplex
 * allows an Action to be started manually via its start method like an
 * asynchronous Action, but run synchronously so that it is completed by the
 * time the start method returns. You can use a Simplex, for example, to create
 * a pool of as-yet-unstarted synchronous Actions.
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

#endif
