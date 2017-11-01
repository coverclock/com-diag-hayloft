/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_PROTOCOL_
#define _H_COM_DIAG_UNITTEST_S3_PROTOCOL_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/Protocol.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft;

typedef Fixture ProtocolTest;

TEST_F(ProtocolTest, Heap) {
	Protocol * pointer = new Protocol;
	EXPECT_NE(pointer, (Protocol*)0);
	delete pointer;
}

TEST_F(ProtocolTest, Stack) {
	Protocol protocol;
}


TEST_F(ProtocolTest, Default) {
	Protocol protocol;
	EXPECT_EQ(protocol.getProtocol(), Protocol::DEFAULT);
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
	EXPECT_EQ(protocolfunction(), Protocol::DEFAULT);
}

TEST_F(ProtocolTest, Settors) {
	Protocol protocol;
	protocol.setProtocol().setProtocol(::S3ProtocolHTTP);
	EXPECT_EQ(protocol.getProtocol(), ::S3ProtocolHTTP);
	EXPECT_EQ(protocol.setProtocol().getProtocol(), Protocol::DEFAULT);
}

}
}
}
}

#endif
