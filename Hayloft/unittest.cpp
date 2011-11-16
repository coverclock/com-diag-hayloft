/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

// Infrastructure

#include "com/diag/lariat/lariat.h"
#include "com/diag/unittest/s3/Environment.h"

// Unit Tests that stand alone.

#if 1
#include "com/diag/unittest/Logger.h"
#include "com/diag/unittest/Packet.h"
#include "com/diag/unittest/Parameter.h"
#include "com/diag/unittest/Size.h"
#endif

// Unit tests that also use libs3, SSL, and CURL.

#if 1
#include "com/diag/unittest/s3/Session.h"
#include "com/diag/unittest/s3/Credentials.h"
#include "com/diag/unittest/s3/Protocol.h"
#include "com/diag/unittest/s3/Style.h"
#include "com/diag/unittest/s3/Region.h"
#include "com/diag/unittest/s3/Access.h"
#include "com/diag/unittest/s3/Context.h"
#include "com/diag/unittest/s3/Queue.h"
#include "com/diag/unittest/s3/Properties.h"
#include "com/diag/unittest/s3/Conditions.h"
#endif

// Unit tests that also use AWS and Buckets.

#if 0
#include "com/diag/unittest/s3/Bucket.h"
#endif

// Unit tests that also use AWS and Objects.

#if 1
#include "com/diag/unittest/s3/Object.h"
#endif

int main(int argc, char ** argv, char **envp) {
	return ::com::diag::lariat::main(argc, argv, envp);
}
