/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_UNITTEST_PACKET_H_
#define _COM_DIAG_UNITTEST_PACKET_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/hayloft/Packet.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

TEST(PacketTest, Full) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketTest, FullConsumeOnce) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	char buffer[sizeof(data)];
	EXPECT_EQ(packetdata.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetdata.size(), (size_t)0);
}

TEST(PacketTest, FullConsumeMany) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetdata.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetdata.size(), (size_t)0);
}

TEST(PacketTest, FullBufferSize) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_EQ(std::memcmp(data, packetdata.buffer(), packetdata.size()), 0);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	packetdata.empty();
	EXPECT_EQ(packetdata.size(), (size_t)0);
}

TEST(PacketTest, Empty) {
}

}
}
}

#endif
