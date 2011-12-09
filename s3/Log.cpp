/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Log.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Log::Log(const Bucket & bucket, const Bucket & log, const Grant & grant, const char * keyprefix)
: Grant(bucket, grant)
, target(log.getCanonical())
, prefix(set(keyprefix))
{
	initialize();
}

Log::Log(const Bucket & bucket, const Multiplex & multiplex, const Bucket & log, const Grant & grant, const char * keyprefix)
: Grant(bucket, multiplex, grant)
, target(log.getCanonical())
, prefix(set(keyprefix))
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
}
