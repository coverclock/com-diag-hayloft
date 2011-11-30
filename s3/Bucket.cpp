/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/show.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

static const char * canonicalize(const char * name, std::string ** canonical, const Session & session) {
	*canonical = new std::string(name);
	session.canonicalize(**canonical);
	return (*canonical)->c_str();
}

Bucket::Bucket(const char * bucketname, const Context & context, const Session & session)
: Container(context.getId(), context.getSecret(), session.getEndpoint(), canonicalize(bucketname, &temporary, session), context.getProtocol(), context.getStyle())
, name(bucketname)
, region(context.getRegion())
, access(context.getAccess())
{
	initialize();
}

Bucket::Bucket(const char * bucketname, Multiplex & multiplex, const Context & context, const Session & session)
: Container(context.getId(), context.getSecret(), session.getEndpoint(), canonicalize(bucketname, &temporary, session), context.getProtocol(), context.getStyle(), multiplex)
, name(bucketname)
, region(context.getRegion())
, access(context.getAccess())
{
	initialize();
}

Bucket::~Bucket() {}

void Bucket::initialize() {
	delete temporary;
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		logger.debug("Bucket@%p: name=\"%s\"\n", this, name.c_str());
		logger.debug("Bucket@%p: region=\"%s\"\n", this, region.c_str());
		logger.debug("Bucket@%p: access=%d=\"%s\"\n", this, access, tostring(access));
	}
}

}
}
}
}
