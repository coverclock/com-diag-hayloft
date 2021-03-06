/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/Plex.h"
#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {

static const char * canonicalize(const char * name, std::string ** canonical, const Session & session) {
	*canonical = new std::string(name);
	session.canonicalize(**canonical);
	return (*canonical)->c_str();
}

Bucket::Bucket()
: Container("", "", "", "", Protocol::DEFAULT, Style::DEFAULT)
, region(Region::DEFAULT())
, access(Access::DEFAULT)
, temporary(0)
{
	initialize();
}

Bucket::Bucket(const char * bucketname, const Context & context, const Session & session)
: Container(context.getId(), context.getSecret(), session.getEndpoint(), canonicalize(bucketname, &temporary, session), context.getProtocol(), context.getStyle())
, name(bucketname)
, region(context.getRegion())
, access(context.getAccess())
{
	initialize();
}

Bucket::Bucket(const char * bucketname, const Plex & plex, const Context & context, const Session & session)
: Container(context.getId(), context.getSecret(), session.getEndpoint(), canonicalize(bucketname, &temporary, session), context.getProtocol(), context.getStyle(), plex)
, name(bucketname)
, region(context.getRegion())
, access(context.getAccess())
{
	initialize();
}

Bucket::Bucket(const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, name(bucket.getName())
, region(bucket.getRegion())
, access(bucket.getAccess())
, temporary(0)
{
	initialize();
}

Bucket::Bucket(const Bucket & bucket, const Plex & plex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), plex)
, name(bucket.getName())
, region(bucket.getRegion())
, access(bucket.getAccess())
, temporary(0)
{
	initialize();
}

Bucket::~Bucket() {}

void Bucket::initialize() {
	delete temporary;
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(::com::diag::grandote::MaskableLogger::DEBUG)) {
		logger.debug("Bucket@%p: name=\"%s\"\n", this, name.c_str());
		logger.debug("Bucket@%p: region=\"%s\"\n", this, region.c_str());
		logger.debug("Bucket@%p: access=%d=\"%s\"\n", this, access, tostring(access));
	}
}

bool Bucket::isValid() const {
	bool result = true;
	Status status = ::S3_validate_bucket_name(canonical.c_str(), style);
	if (status != ::S3StatusOK) {
		::com::diag::grandote::MaskableLogger::instance().warning("Bucket@%p: S3_validate_bucket_name failed! name=\"%s\"[%zu] status=%d=\"%s\"\n", this, name.c_str(), name.length(), status, tostring(status));
		result = false;
	}
	return result;
}

}
}
}
