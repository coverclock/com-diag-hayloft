/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_ENDPOINT_
#define _H_COM_DIAG_UNITTEST_S3_ENDPOINT_

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
#include "com/diag/hayloft/s3/Endpoint.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture EndpointTest;

TEST_F(EndpointTest, Heap) {
	Endpoint * pointer = new Endpoint;
	EXPECT_NE(pointer, (Endpoint*)0);
	delete pointer;
}

TEST_F(EndpointTest, Stack) {
	Endpoint endpoint;
}

TEST_F(EndpointTest, Default) {
	Endpoint endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::DEFAULT()), 0);
}

TEST_F(EndpointTest, Ireland) {
	EndpointIreland endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::EUROPEAN_UNION_WEST_1()), 0);
}

TEST_F(EndpointTest, NorthernCalifornia) {
	EndpointNorthernCalifornia endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::UNITED_STATES_WEST_1()), 0);
}

TEST_F(EndpointTest, Singapore) {
	EndpointSingapore endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::ASIA_PACIFIC_SOUTHEAST_1()), 0);
}

TEST_F(EndpointTest, Tokyo) {
	EndpointTokyo endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::ASIA_PACIFIC_NORTHEAST_1()), 0);
}

TEST_F(EndpointTest, UnitedStates) {
	EndpointUnitedStates endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::UNITED_STATES_CLASSIC()), 0);
}

TEST_F(EndpointTest, NorthernVirginia) {
	EndpointNorthernVirginia endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::UNITED_STATES_EAST_1()), 0);
}

TEST_F(EndpointTest, Oregon) {
	EndpointOregon endpoint;
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::UNITED_STATES_WEST_2()), 0);
}

static const char * endpointfunction(const Endpoint & constraint = Endpoint()) {
	return constraint.getEndpoint();
}

TEST_F(EndpointTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(endpointfunction(), Endpoint::DEFAULT()), 0);
}

TEST_F(EndpointTest, Settors) {
	Endpoint endpoint;
	endpoint.setEndpoint().setEndpoint(Endpoint::ASIA_PACIFIC_NORTHEAST_1());
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::ASIA_PACIFIC_NORTHEAST_1()), 0);
	endpoint.setEndpoint();
	ASSERT_NE(endpoint.getEndpoint(), (char *)0);
	EXPECT_EQ(std::strcmp(endpoint.getEndpoint(), Endpoint::DEFAULT()), 0);
}

}
}
}
}

#endif
