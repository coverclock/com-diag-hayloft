/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Region.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const size_t Region::LENGTH;

Region::Region(const char * re)
: region((re != 0) ? re : "")
{
	Logger::instance().debug("Region@%p: region=\"%s\"\n", this, region.c_str());
}

}
}
}
}