/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

// Infrastructure.

#include "gtest/gtest.h"
#include "com/diag/lariat/lariat.h"
#include "com/diag/unittest/s3/Environment.h"

// Unit Tests that stand alone.

#if 1
#include "com/diag/unittest/Seconds.h"
#include "com/diag/unittest/Logger.h"
#include "com/diag/unittest/Packet.h"
#include "com/diag/unittest/Parameter.h"
#include "com/diag/unittest/size.h"
#endif

// Unit tests that also use LIBS3, CURL, SSL, and CRYPTO. Running valgrind, my
// favorite tool, reveals that some of these libraries are not so well behaved.
// (Maybe this is a latent issue in my code, but so far I don't think so.)

#if 1
#include "com/diag/unittest/s3/sizeof.h"
#include "com/diag/unittest/s3/Endpoint.h"
#include "com/diag/unittest/s3/Session.h"
#include "com/diag/unittest/s3/Credentials.h"
#include "com/diag/unittest/s3/Protocol.h"
#include "com/diag/unittest/s3/Style.h"
#include "com/diag/unittest/s3/Region.h"
#include "com/diag/unittest/s3/Access.h"
#include "com/diag/unittest/s3/Context.h"
#include "com/diag/unittest/s3/Multiplex.h"
#include "com/diag/unittest/s3/Properties.h"
#include "com/diag/unittest/s3/Conditions.h"
#include "com/diag/unittest/s3/Selection.h"
#include "com/diag/unittest/s3/BucketBase.h"
#include "com/diag/unittest/s3/ObjectBase.h"
#endif

// Unit tests that use S3.

#if 1
#include "com/diag/unittest/s3/Bucket.h"
#endif

#if 1
#include "com/diag/unittest/s3/Object.h"
#endif

#if 1
#include "com/diag/unittest/s3/convergence.h"
#endif

#if 1
#include "com/diag/unittest/s3/Grant.h"
#endif

class Logger : public ::com::diag::hayloft::Logger {
public:
	static void cleanup() { delete singleton; singleton = 0; }
};

class Session : public ::com::diag::hayloft::s3::Session {
public:
	static void cleanup() { delete singleton; singleton = 0; }
};

int main(int argc, char ** argv, char **envp) {
	int rc = ::com::diag::lariat::main(argc, argv, envp);
	Logger::cleanup(); // For valgrind.
	Session::cleanup(); // For valgrind.
	return rc;
}
