/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <ctype.h>
#include "com/diag/hayloft/Session.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/Mutex.h"
#include "com/diag/grandote/CriticalSection.h"
#include "com/diag/grandote/target.h"

namespace com {
namespace diag {
namespace hayloft {

static ::com::diag::grandote::Mutex instancemutex;

static ::com::diag::grandote::Mutex initializationmutex;

static Session * instant = 0;

Session * Session::singleton = 0;

/*
 * References:
 *
 * Amazon Web Services, AMAZON SIMPLE STORAGE SERVICE DEVELOPER GUIDE,
 * API Version 2006-03-01, "Bucket Restrictions and Limitations", p. 83
 *
 * Amazon Simple Storage Service Team, BEST PRACTICES FOR USING AMAZON S3,
 * 2008-11-10
 */
static void convert_to_lower_case(std::string & upper) {
	size_t length = upper.length();
	for (int ii = 0; ii < length; ++ii) {
		if (isupper(upper[ii])) {
			upper.replace(ii, 1, 1, tolower(upper[ii]));
		}
	}
}

Session & Session::factory() {
    return (*(new Session));
}

Session & Session::instance(Session & that) {
	::com::diag::grandote::CriticalSection guard(instancemutex);
	singleton = &that;
    return *singleton;
}

Session & Session::instance() {
	::com::diag::grandote::CriticalSection guard(instancemutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}

Session::Session(const char * bucketSuffix, const char * userAgentInfo, const Endpoint & endPoint, int flags)
: bucketsuffix(::com::diag::grandote::set(bucketSuffix, BUCKET_SUFFIX_ENV(), BUCKET_SUFFIX_STR()))
, useragent(::com::diag::grandote::set(userAgentInfo, USER_AGENT_ENV(), USER_AGENT_STR()))
, endpoint(endPoint.getEndpoint())
, status(::S3StatusOK)
{
	{
		::com::diag::grandote::CriticalSection guard(initializationmutex);
		status = ::S3_initialize(useragent.c_str(), flags, endpoint.c_str());
	}
	convert_to_lower_case(bucketsuffix);
	convert_to_lower_case(useragent);
	convert_to_lower_case(endpoint);
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
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
	if (!canonical.empty()) {
		convert_to_lower_case(canonical);
		if (!bucketsuffix.empty()) {
			canonical += bucketsuffix;
		}
	}
	return canonical.c_str();
}

const char * Session::decanonicalize(std::string & decanonical) const {
	size_t dlength = decanonical.length();
	size_t slength = bucketsuffix.length();
	if ((dlength > slength) && (slength > 0)) {
		if (decanonical.compare(dlength - slength, slength, bucketsuffix) == 0) {
			decanonical.erase(dlength - slength, slength);
		}
	}
	return decanonical.c_str();
}

}
}
}
