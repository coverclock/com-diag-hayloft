/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/Log.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/set.h"

namespace com {
namespace diag {
namespace hayloft {

Log::Log(const Bucket & bucket, const Bucket & log, const char * keyprefix, const Grant & grant)
: Grant(bucket, grant)
, target(log.getCanonical())
, prefix(::com::diag::grandote::set(keyprefix))
{
	initialize();
}

Log::Log(const Bucket & bucket, const Plex & plex, const Bucket & log, const char * keyprefix, const Grant & grant)
: Grant(bucket, plex, grant)
, target(log.getCanonical())
, prefix(::com::diag::grandote::set(keyprefix))
{
	initialize();
}


Log::~Log() {
}

void Log::initialize() {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("Log@%p: target=\"%s\"\n", this, target.c_str());
		logger.debug("Log@%p: prefix=\"%s\"\n", this, prefix.c_str());
	}
}

}
}
}
