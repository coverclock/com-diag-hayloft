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
#include "com/diag/desperado/FileInput.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

TEST(PacketDataTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketData * pdp = new PacketData(data, sizeof(data));
	delete pdp;
}

TEST(PacketDataTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::EITHER);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::APPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::PREPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, ConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetdata.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, ConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetdata.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataTest, BufferSize) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.size(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetdata.buffer(), packetdata.size()), 0);
	packetdata.clear();
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketDataDynamicTest, Heap) {
	static const size_t SIZE = 256;
	PacketDataDynamic::Datum * data = new PacketDataDynamic::Datum [SIZE];
	PacketDataDynamic * pddp = new PacketDataDynamic(data, sizeof(data));
	delete pddp;
}

TEST(PacketBufferTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketBuffer * pbp = new PacketBuffer(data, SIZE);
	delete pbp;
}

TEST(PacketBufferDynamicTest, Heap1) {
	static const size_t SIZE = 256;
	PacketBufferDynamic::Datum * data = new PacketBufferDynamic::Datum[SIZE];
	PacketBufferDynamic * pbdp = new PacketBufferDynamic(data, SIZE);
	delete pbdp;
}

TEST(PacketBufferDynamicTest, Heap2) {
	static const size_t SIZE = 256;
	PacketBufferDynamic * pbdp = new PacketBufferDynamic(SIZE);
	delete pbdp;
}

TEST(PacketBufferDynamicTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, AppendOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, AppendOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, AppendManyAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t suffix = SIZE;
	for (size_t ii = 0; ii < sizeof(data); ++ii) {
		EXPECT_EQ(packetbuffer.append(&data[ii], sizeof(data[ii])), sizeof(data[ii]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[ii]);
		suffix -= sizeof(data[ii]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), ZERO);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependManyPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE;
	for (size_t ii = sizeof(data); ii > 0; --ii) {
		EXPECT_EQ(packetbuffer.prepend(&data[ii-1], sizeof(data[ii-1])), sizeof(data[ii-1]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[ii-1]);
		prefix -= sizeof(data[ii-1]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.size()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependAppendConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE / 2;
	size_t suffix = SIZE / 2;
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[jj]);
		prefix -= sizeof(data[jj]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetbuffer.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE / 2;
	size_t suffix = SIZE / 2;
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[jj]);
		prefix -= sizeof(data[jj]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
		EXPECT_EQ(packetbuffer.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
		size -= sizeof(buffer[ii]);
		prefix += sizeof(buffer[ii]);
		EXPECT_EQ(packetbuffer.size(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, AppendOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketBufferDynamicTest, PrependOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST(PacketTest, Heap) {
	Packet * packet = new Packet;
	delete packet;
}

TEST(PacketTest, PrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC);
	packet.show(2);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2);
}

}
}
}

#endif
