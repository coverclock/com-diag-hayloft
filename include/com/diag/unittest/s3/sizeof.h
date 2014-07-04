/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_SIZEOF_
#define _H_COM_DIAG_UNITTEST_S3_SIZEOF_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"

#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/BucketDelete.h"
#include "com/diag/hayloft/s3/BucketHead.h"
#include "com/diag/hayloft/s3/BucketManifest.h"
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/s3/GrantGet.h"
#include "com/diag/hayloft/s3/GrantSet.h"
#include "com/diag/hayloft/s3/LogGet.h"
#include "com/diag/hayloft/s3/LogSet.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/ObjectCopy.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/hayloft/s3/ObjectHead.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/hayloft/s3/ServiceManifest.h"
#include "com/diag/hayloft/s3/Simplex.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture SizeofTest;

#define PRINT_SIZEOF(_TYPE_) printf("sizeof(%s)=%zu\n", #_TYPE_, sizeof(_TYPE_))

TEST_F(SizeofTest, Sizeof) {
	PRINT_SIZEOF(Access);
	PRINT_SIZEOF(Action);
	PRINT_SIZEOF(Bucket);
	PRINT_SIZEOF(BucketCreate);
	PRINT_SIZEOF(BucketDelete);
	PRINT_SIZEOF(BucketHead);
	PRINT_SIZEOF(BucketManifest);
	PRINT_SIZEOF(Complex);
	PRINT_SIZEOF(Conditions);
	PRINT_SIZEOF(Container);
	PRINT_SIZEOF(Context);
	PRINT_SIZEOF(Credentials);
	PRINT_SIZEOF(Endpoint);
	PRINT_SIZEOF(Grant);
	PRINT_SIZEOF(GrantGet);
	PRINT_SIZEOF(GrantSet);
	PRINT_SIZEOF(Multiplex);
	PRINT_SIZEOF(Log);
	PRINT_SIZEOF(LogGet);
	PRINT_SIZEOF(LogSet);
	PRINT_SIZEOF(Object);
	PRINT_SIZEOF(ObjectCopy);
	PRINT_SIZEOF(ObjectDelete);
	PRINT_SIZEOF(ObjectGet);
	PRINT_SIZEOF(ObjectHead);
	PRINT_SIZEOF(ObjectPut);
	PRINT_SIZEOF(Plex);
	PRINT_SIZEOF(Properties);
	PRINT_SIZEOF(Protocol);
	PRINT_SIZEOF(Region);
	PRINT_SIZEOF(Selection);
	PRINT_SIZEOF(Service);
	PRINT_SIZEOF(ServiceManifest);
	PRINT_SIZEOF(Session);
	PRINT_SIZEOF(Simplex);
	PRINT_SIZEOF(Style);
}

}
}
}
}

#endif
