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
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture PacketDataTest;

TEST_F(PacketDataTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketData * pdp = new PacketData(data, sizeof(data));
	delete pdp;
}

TEST_F(PacketDataTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::EITHER);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::APPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::PREPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, ConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetdata.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, ConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetdata.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, BufferSize) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetdata.buffer(), packetdata.length()), 0);
	packetdata.clear();
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), sizeof(data));
	EXPECT_EQ(packetdata.suffix(), sizeof(data));
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

typedef Fixture PacketDataDynamicTest;

TEST_F(PacketDataDynamicTest, Heap) {
	static const size_t SIZE = 256;
	PacketDataDynamic::Datum * data = new PacketDataDynamic::Datum [SIZE];
	PacketDataDynamic * pddp = new PacketDataDynamic(data, sizeof(data));
	delete pddp;
}

typedef Fixture PacketBufferTest;

TEST_F(PacketBufferTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketData * pbp = new PacketBuffer(data, SIZE);
	delete pbp;
}

typedef Fixture PacketBufferDynamicTest;

TEST_F(PacketBufferDynamicTest, Heap1) {
	static const size_t SIZE = 256;
	PacketBufferDynamic::Datum * data = new PacketBufferDynamic::Datum[SIZE];
	PacketData * pbdp = new PacketBufferDynamic(data, SIZE);
	delete pbdp;
}

TEST_F(PacketBufferDynamicTest, Heap2) {
	static const size_t SIZE = 256;
	PacketData * pbdp = new PacketBufferDynamic(SIZE);
	delete pbdp;
}

TEST_F(PacketBufferDynamicTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendManyAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
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
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), ZERO);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependManyPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
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
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
	}
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependAppendConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
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
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetbuffer.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
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
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
		EXPECT_EQ(packetbuffer.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
		size -= sizeof(buffer[ii]);
		prefix += sizeof(buffer[ii]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), sizeof(data));
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

typedef Fixture PacketTest;

TEST_F(PacketTest, HeapAndShow) {
	Packet * packet = new Packet;
	ASSERT_NE(packet, (Packet*)0);
	packet->show(0, &PacketTest::errput);
	delete packet;
}

TEST_F(PacketTest, EitherPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::EITHER);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, PrependPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::PREPEND);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, AppendPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::APPEND);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, EitherAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::EITHER);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, PrependAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::PREPEND);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, AppendAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::APPEND);
	packet.show(2, &PacketTest::errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &PacketTest::errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &PacketTest::errput);
}

TEST_F(PacketTest, MixedBag) {
	Packet * packet = new Packet;
	PacketDataDynamic::Datum * data2 = new PacketDataDynamic::Datum [3];
	for (int ii = 0; ii < 3; ++ii) { data2[ii] = 'j' + ii; }
	PacketDataDynamic * pdd = new PacketDataDynamic(data2, 3);
	packet->append(*pdd);
	struct Data1 { char data[7]; } data1 = { { 'c', 'd', 'e', 'F', 'g', 'h', 'i' } };
	PacketData * pd = new PacketData(&data1, sizeof(data1));
	packet->prepend(*pd);
	struct Data3 { char data[11]; } data3;
	PacketBuffer * pb = new PacketBuffer(&data3, sizeof(data3), PacketBuffer::APPEND);
	packet->append(*pb);
	PacketBufferDynamic::Datum * data4 = new PacketBufferDynamic::Datum [2];
	PacketBufferDynamic * pbd1 = new PacketBufferDynamic(data4, 2, PacketBufferDynamic::PREPEND);
	packet->prepend(*pbd1);
	EXPECT_EQ(packet->prepend("ab", 2), (size_t)2);
	PacketBufferDynamic * pbd2 = new PacketBufferDynamic(5, PacketBufferDynamic::APPEND);
	packet->append(*pbd2);
	EXPECT_EQ(packet->append("xyz!", 5), (size_t)5);
	data1.data[3] = 'f'; // Modification!
	EXPECT_EQ(pb->append("mnopqrstuvw", 11), (size_t)11); // Insertion!
	char buffer[28];
	EXPECT_EQ(packet->consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::strncmp(buffer, "abcdefghijklmnopqrstuvwxyz!", sizeof(buffer)), 0);
	delete packet;
}

typedef Fixture PacketInputOutputTest;

TEST_F(PacketInputOutputTest, Block) {
	static const size_t SIZE = 5 * 11 * 4;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	static const size_t OUTPUT = 5;
	for (size_t ii = 0; ii < sizeof(data); ii += OUTPUT) {
		EXPECT_EQ((packet.output())(&(data[ii]), 1, OUTPUT), OUTPUT);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[sizeof(data)];
	static const size_t INPUT = 11;
	for (size_t ii = 0; ii < sizeof(buffer); ii += INPUT) {
		EXPECT_EQ((packet.input())(&(buffer[ii]), 1, INPUT), INPUT);
	}
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
}

TEST_F(PacketInputOutputTest, Character) {
	static const size_t SIZE = 5 * 11 * 4;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < sizeof(data); ++ii) {
		EXPECT_EQ((packet.output())((int)data[ii]), (int)data[ii]);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[sizeof(data)];
	int ch;
	int ch2;
	for (size_t ii = 0; ii < sizeof(buffer);++ii) {
		EXPECT_FALSE(packet.empty());
		ch = (packet.input())();
		EXPECT_NE(ch, EOF);
		ch2 = (packet.input())(ch);
		EXPECT_EQ(ch, ch2);
		EXPECT_FALSE(packet.empty());
		ch = (packet.input())();
		EXPECT_NE(ch, EOF);
		EXPECT_EQ(ch, ch2);
		buffer[ii] = ch;
	}
	ch = (packet.input())();
	EXPECT_EQ(ch, EOF);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
}

static const char * SONNET99[] = {
	"SONNET 99\n",
	"Love is not all: it is not meat nor drink\n",
	"Nor slumber nor a roof against the rain;\n",
	"Nor yet a floating spar to men that sink\n",
	"And rise and sink and rise and sink again;\n",
	"Love can not fill the thickened lung with breath,\n",
	"Nor clean the blood, nor set the fractured bone;\n",
	"Yet many a man is making friends with death\n",
	"Even as I speak, for lack of love alone.\n",
	"It well may be that in a difficult hour,\n",
	"Pinned down by pain and moaning for release,\n",
	"Or nagged by want past resolution's power,\n",
	"I might be driven to sell your love for peace,\n",
	"Or trade the memory of this night for food.\n",
	"It well may be. I do not think I would.\n",
	"-- Edna St. Vincent Millay"
};

TEST_F(PacketInputOutputTest, String) {
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < countof(SONNET99); ++ii) {
		size_t length = std::strlen(SONNET99[ii]);
		EXPECT_LE((packet.output())(SONNET99[ii]), length);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[countof(SONNET99)][64];
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		(packet.input())(buffer[ii], sizeof(buffer[ii]));
	}
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		EXPECT_EQ(std::strncmp(SONNET99[ii], buffer[ii], sizeof(buffer[ii])), 0);
	}
}

TEST_F(PacketInputOutputTest, Formatted) {
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	::com::diag::desperado::Print print(packet.output());
	for (size_t ii = 0; ii < countof(SONNET99); ++ii) {
		print("%s", SONNET99[ii]);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[countof(SONNET99)][64];
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		(packet.input())(buffer[ii], sizeof(buffer[ii]));
	}
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		EXPECT_EQ(std::strncmp(SONNET99[ii], buffer[ii], sizeof(buffer[ii])), 0);
	}
}

static const char RICHARDII[] = {
	"This royal throne of kings, this sceptred isle,\n"
	"This earth of majesty, this seat of Mars,\n"
	"This other Eden, demi-paradise,\n"
	"This fortress built by Nature for herself\n"
	"Against infection and the hand of war,\n"
	"This happy breed of men, this little world,\n"
	"This precious stone set in the silver sea,\n"
	"Which serves it in the office of a wall\n"
	"Or as a moat defensive to a house,\n"
	"Against the envy of less happier lands,--\n"
	"This blessed plot, this earth, this realm, this England.\n"
	"-- William Shakespeare, KING RICHARD II, Act 2 scene 1\n"
};

TEST_F(PacketInputOutputTest, SourceSinkBuffer) {
	::com::diag::desperado::DataInput datainput(RICHARDII, sizeof(RICHARDII));
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(packet.source(datainput), sizeof(RICHARDII));
	EXPECT_FALSE(packet.empty());
	char buffer[sizeof(RICHARDII)];
	::com::diag::desperado::BufferOutput bufferoutput(buffer, sizeof(buffer));
	EXPECT_EQ(packet.sink(bufferoutput), sizeof(RICHARDII));
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::strncmp(RICHARDII, buffer, sizeof(buffer)), 0);
}

TEST_F(PacketInputOutputTest, SourceSinkPathFile) {
	struct ::stat status;
	EXPECT_EQ(::stat(__FILE__, &status), 0);
	::com::diag::desperado::PathInput input(__FILE__, "r");
	char name[] = "/tmp/PacketTest.SourceSinkPathFile.XXXXXX";
	int fd = ::mkstemp(name);
	ASSERT_TRUE(fd > 0);
	::com::diag::desperado::DescriptorOutput output(fd);
	Packet packet;
	EXPECT_TRUE(packet.empty());
	size_t sourced = packet.source(input);
	EXPECT_FALSE(packet.empty());
	EXPECT_EQ(sourced, (size_t)status.st_size);
	EXPECT_EQ(packet.length(), (size_t)status.st_size);
	size_t sunk = packet.sink(output);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(sunk, sourced);
	EXPECT_EQ(::close(fd), 0);
	std::string command = "diff ";
	command += __FILE__;
	command += " ";
	command += name;
	EXPECT_EQ(std::system(command.c_str()), 0);
	EXPECT_EQ(::unlink(name), 0);
}

}
}
}

#endif
