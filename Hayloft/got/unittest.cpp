/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

// Infrastructure.

#include "gtest/gtest.h"
#include "com/diag/lariat/lariat.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/grandote/generics.h"
#include "Environment.h"
#include "Fixture.h"

#if 0
#define COM_DIAG_HAYLOFT_UNITTEST_CONSISTENCY
#endif

namespace com {
namespace diag {
namespace unittest {

static const int LIMIT = Fixture::limit(intmaxof(int));
static const Milliseconds TIMEOUT = Fixture::timeout(60000); /* one minute */

}
}
}

// Unit tests that also use libs3, libcurl, libssl, and libcrypto. Running
// valgrind reveals that some of these libraries are not so well behaved.
// Maybe this is a latent issue in my code, but so far I don't think so. It
// appears to be a known issue in libcrypto in which uninitialized memory is
// deliberately used while trying to increase the size of the entropy pool.

#if 1
#include "sizeof.h"
#include "Endpoint.h"
#include "Session.h"
#include "Credentials.h"
#include "Protocol.h"
#include "Style.h"
#include "Region.h"
#include "Access.h"
#include "Context.h"
#include "Multiplex.h"
#include "Properties.h"
#include "Conditions.h"
#include "Selection.h"
#include "BucketBase.h"
#include "ObjectBase.h"
#include "GrantBase.h"
#include "LogBase.h"
#endif

// Unit tests that use S3, perhaps more properly called functional tests.

#if 1
#include "Bucket.h"
#include "Object.h"
#include "Grant.h"
#include "Log.h"
#include "Complex.h"
#include "Manifest.h"
#endif

// Experimental tests regarding eventual consistency.

#ifdef COM_DIAG_HAYLOFT_UNITTEST_CONSISTENCY
#include "convergence.h"
#endif

// Hooks to clean up singletons just for valgrind.

class Platform : public ::com::diag::grandote::Platform {
public:
	static void finalize() { delete singleton; singleton = 0; }
};

class MaskableLogger : public ::com::diag::grandote::MaskableLogger {
public:
	static void finalize() { delete singleton; singleton = 0; }
};

class Session : public ::com::diag::hayloft::Session {
public:
	static void finalize() { delete singleton; singleton = 0; }
};

int main(int argc, char ** argv, char **envp) {
	int rc = ::com::diag::lariat::main(argc, argv, envp);
	Session::finalize();
	MaskableLogger::finalize();
	Platform::finalize();
	return rc;
}
