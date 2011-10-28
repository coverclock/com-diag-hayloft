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

TEST(PacketDataTest, Heap) {
	char data[256];
	PacketData * pdp = new PacketData(data, sizeof(data));
	delete pdp;
}

TEST(PacketDataTest, Initial) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), (size_t)0);
	EXPECT_EQ(packetdata.suffix(), (size_t)0);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketDataTest, InitialLeft) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), 1);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), (size_t)0);
	EXPECT_EQ(packetdata.suffix(), (size_t)0);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketDataTest, ConsumeOnce) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	char buffer[sizeof(data)];
	EXPECT_EQ(packetdata.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetdata.size(), (size_t)0);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketDataTest, ConsumeMany) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetdata.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetdata.size(), (size_t)0);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketDataTest, BufferSize) {
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(std::memcmp(data, packetdata.buffer(), packetdata.size()), 0);
	packetdata.empty();
	EXPECT_EQ(packetdata.size(), (size_t)0);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, Heap) {
	PacketBuffer * pbp = new PacketBuffer(256);
	delete pbp;
}

TEST(PacketBufferTest, Initial) {
	PacketBuffer packetbuffer(256);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, InitialLeft) {
	PacketBuffer packetbuffer(256, 1);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, AppendOnce) {
	PacketBuffer packetbuffer(256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, AppendMany) {
	PacketBuffer packetbuffer(256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	for (size_t ii = 0; ii < sizeof(data); ++ii) {
		EXPECT_EQ(packetbuffer.append(&data[ii], sizeof(data[ii])), sizeof(data[ii]));
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependOnce) {
	PacketBuffer packetbuffer(256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependOnceLeft) {
	PacketBuffer packetbuffer(256, 1);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependManyLeft) {
	PacketBuffer packetbuffer(256, 1);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	for (size_t ii = sizeof(data); ii > 0; --ii) {
		EXPECT_EQ(packetbuffer.prepend(&data[ii-1], sizeof(data[ii-1])), sizeof(data[ii-1]));
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), (size_t)0);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependAppendConsumeOnce) {
	PacketBuffer packetbuffer(256, 2);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetbuffer.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependAppendConsumeMany) {
	PacketBuffer packetbuffer(256, 2);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetbuffer.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, AppendEmpty) {
	PacketBuffer packetbuffer(256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	packetbuffer.empty();
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

TEST(PacketBufferTest, PrependEmpty) {
	PacketBuffer packetbuffer(256);
	char data[256];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), (size_t)0);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)0);
	packetbuffer.empty();
	EXPECT_EQ(packetbuffer.size(), (size_t)0);
	EXPECT_EQ(packetbuffer.prefix(), (size_t)256);
	EXPECT_EQ(packetbuffer.suffix(), (size_t)256);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), (size_t)0);
}

}
}
}

#endif
