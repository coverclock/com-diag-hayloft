/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_THREAD_
#define _H__COM_DIAG_UNITTEST_S3_THREAD_

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
#include "com/diag/hayloft/Thread.h"
#include "com/diag/desperado/CriticalSection.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture ThreadTest;

static ::com::diag::desperado::Mutex myMutex;

class ThreadOne : public Thread {
public:
	int * variable;
	explicit ThreadOne(int & shared) : variable(&shared) { *variable = 1; }
	virtual void run() { ::com::diag::desperado::CriticalSection guard(myMutex); *variable = 3; }
};

TEST_F(ThreadTest, Idle) {
	int variable = 0;
	ThreadOne thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 1);
}

TEST_F(ThreadTest, Join) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadOne thread(variable);
	EXPECT_EQ(variable, 1);
	{
		::com::diag::desperado::CriticalSection guard(myMutex);
		variable = 2;
		EXPECT_EQ(variable, 2);
		EXPECT_EQ(thread.start(), 0);
		EXPECT_EQ(variable, 2);
		platform.yield(platform.frequency());
		EXPECT_EQ(variable, 2);
	}
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
}

class ThreadTwo : public Thread {
public:
	int * variable;
	explicit ThreadTwo(int & shared) : variable(&shared) { *variable = 1; }
	virtual void run() { while (true) { *variable = 3;  yield(); } }
};

TEST_F(ThreadTest, Cancel) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadOne thread(variable);
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
}

}
}
}

#endif
