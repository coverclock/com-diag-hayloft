/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_CONTEXT
#define _H__COM_DIAG_UNITTEST_S3_CONTEXT

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Environment.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ContextTest;

TEST_F(ContextTest, Heap) {
	Context * pointer = new Context;
	ASSERT_NE(pointer, (Context*)0);
	const Credentials & credentials = pointer->getCredentials();
	const LocationConstraint & constraint = pointer->getLocationConstraint();
	const Protocol & protocol = pointer->getProtocol();
	const UniversalResourceIdentifierStyle & style = pointer->getUniversalResourceIdentifierStyle();
    ::com::diag::desperado::Print printf(::com::diag::desperado::Platform::instance().error());
    printf("context=%p\n", pointer);
    printf("credentials=%p\n", &credentials);
    printf("contraint=%p\n", &constraint);
    printf("protocol=%p\n", &protocol);
    printf("style=%p\n", &style);
	delete pointer;
}

TEST_F(ContextTest, Stack) {
	Context context;
	const Credentials & credentials = context.getCredentials();
	const LocationConstraint & constraint = context.getLocationConstraint();
	const Protocol & protocol = context.getProtocol();
	const UniversalResourceIdentifierStyle & style = context.getUniversalResourceIdentifierStyle();
    ::com::diag::desperado::Print printf(::com::diag::desperado::Platform::instance().error());
    printf("context=%p\n", &context);
    printf("credentials=%p\n", &credentials);
    printf("contraint=%p\n", &constraint);
    printf("protocol=%p\n", &protocol);
    printf("style=%p\n", &style);
}

}
}
}
}

#endif
