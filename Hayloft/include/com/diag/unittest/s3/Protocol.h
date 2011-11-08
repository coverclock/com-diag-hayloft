/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_PROTOCOL
#define _H_COM_DIAG_UNITTEST_S3_PROTOCOL

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstring>
#include "gtest/gtest.h"
#include "com/diag/unittest/s3/Fixture.h"
#include "com/diag/hayloft/s3/Protocol.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture ProtocolTest;

TEST_F(ProtocolTest, Heap) {
	Protocol * pointer = new Protocol;
	EXPECT_NE(pointer, (Protocol*)0);
	delete pointer;
}

TEST_F(ProtocolTest, Default) {
	Protocol protocol;
	EXPECT_EQ(protocol.getProtocol(), ::S3ProtocolHTTPS);
}

TEST_F(ProtocolTest, Secure) {
	ProtocolSecure protocol;
	EXPECT_EQ(protocol.getProtocol(), ::S3ProtocolHTTPS);
}

TEST_F(ProtocolTest, Unsecure) {
	ProtocolUnsecure protocol;
	EXPECT_EQ(protocol.getProtocol(), ::S3ProtocolHTTP);
}

static int protocolfunction(const Protocol & protocol = Protocol()) {
	return protocol.getProtocol();
}

TEST_F(ProtocolTest, DefaultFunctionArgument) {
	EXPECT_EQ(protocolfunction(), ::S3ProtocolHTTPS);
}

}
}
}
}

#endif
