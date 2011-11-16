/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_QUEUE_
#define _H__COM_DIAG_UNITTEST_S3_QUEUE_

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
#include "com/diag/unittest/Fixture.h"
#include "com/diag/hayloft/s3/Queue.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

typedef Fixture QueueTest;

TEST_F(QueueTest, Heap) {
	Queue * queue = new Queue;
	EXPECT_NE(queue, (Queue*)0);
	EXPECT_TRUE((*queue) == true);
	EXPECT_NE(queue->getRequests(), (S3RequestContext *)0);
	delete queue;
}

TEST_F(QueueTest, Stack) {
	Queue queue;
	EXPECT_TRUE(queue == true);
	EXPECT_NE(queue.getRequests(), (S3RequestContext *)0);
}

TEST_F(QueueTest, All) {
	Queue queue;
	EXPECT_TRUE(queue == true);
	EXPECT_TRUE(queue.all());
}

TEST_F(QueueTest, Once) {
	Queue queue;
	EXPECT_TRUE(queue == true);
	int pending;
	EXPECT_TRUE(queue.once(pending));
	EXPECT_EQ(pending, 0);
}

TEST_F(QueueTest, Ready) {
	Queue queue;
	EXPECT_TRUE(queue == true);
	EXPECT_EQ(queue.ready(), 0);
}

}
}
}
}

#endif
