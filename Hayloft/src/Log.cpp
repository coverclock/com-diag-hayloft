/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
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
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Log@%p: target=\"%s\"\n", this, target.c_str());
		logger.debug("Log@%p: prefix=\"%s\"\n", this, prefix.c_str());
	}
}

}
}
}
