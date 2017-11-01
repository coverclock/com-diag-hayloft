/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Plex.h"
#include "com/diag/grandote/MaskableLogger.h"

namespace com {
namespace diag {
namespace hayloft {

Plex::Plex(Handle * untaken)
: handle(untaken)
{
	initialize();
}

Plex::~Plex() {
}

void Plex::initialize() {
	::com::diag::grandote::MaskableLogger::instance().debug("Plex@%p: handle=%p\n", this, handle);
}

}
}
}
