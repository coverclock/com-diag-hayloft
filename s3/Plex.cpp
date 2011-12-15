/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Plex.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Plex::Plex(::S3RequestContext * untaken)
: requests(untaken)
{
	initialize();
}

Plex::~Plex() {
}

void Plex::initialize() {
	Logger::instance().debug("Plex@%p: requests=%p\n", this, requests);
}

}
}
}
}
