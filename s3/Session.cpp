/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <ctype.h>
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/CriticalSection.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

static ::com::diag::desperado::Mutex mutex;

static Session * instant = 0;

Session * Session::singleton = 0;

Session & Session::factory() {
    return (*(new Session));
}

Session & Session::instance(Session & that) {
	::com::diag::desperado::CriticalSection guard(mutex);
	Session::singleton = &that;
    return *singleton;
}

Session & Session::instance() {
	::com::diag::desperado::CriticalSection guard(mutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}

Session::Session(const char * bucketSuffix, const char * userAgentInfo, const Endpoint & endPoint, int flags)
: bucketsuffix(set(bucketSuffix, BUCKET_SUFFIX_ENV(), BUCKET_SUFFIX_STR()))
, useragent(set(userAgentInfo, USER_AGENT_ENV(), USER_AGENT_STR()))
, endpoint(endPoint.getEndpoint())
, status(::S3_initialize(useragent.c_str(), flags, endpoint.c_str()))
{
	Logger & logger = Logger::instance();
	logger.debug("Session@%p: bucketsuffix=\"%s\"\n", this, bucketsuffix.c_str());
	logger.debug("Session@%p: useragent=\"%s\"\n", this, useragent.c_str());
	logger.debug("Session@%p: endpoint=\"%s\"\n", this, endpoint.c_str());
	logger.debug("Session@%p: flags=0x%x\n", this, flags);
	if (status != ::S3StatusOK) {
		logger.error("Session@%p: S3_initialize failed! status=%d=\"%s\"\n", this, status, tostring(status));
	}
}

Session::~Session() {
	S3_deinitialize();
}

const char * Session::canonicalize(std::string & canonical) const {
	if ((!canonical.empty()) && (!bucketsuffix.empty())) {
		canonical += bucketsuffix;
	}
	// Ref: Amazon Web Services, AMAZON SIMPLE STORAGE SERVICE DEVELOPER GUIDE,
	// API Version 2006-03-01, "Bucket Restrictions and Limitations", p. 83
	for (int ii = 0; ii < canonical.length(); ++ii) {
		if (isupper(canonical[ii])) {
			canonical.replace(ii, 1, 1, tolower(canonical[ii]));
		}
	}
	return canonical.c_str();
}

}
}
}
}
