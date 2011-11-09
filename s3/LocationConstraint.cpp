/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

LocationConstraint::LocationConstraint(const char * locationconstraint)
: constraint((locationconstraint != 0) ? locationconstraint : "")
{
	Logger::instance().debug("LocationConstraint@%p: locationconstraint=\"%s\"\n", this, constraint.c_str());
}

}
}
}
}
