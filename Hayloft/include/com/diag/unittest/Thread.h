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
#include "com/diag/hayloft/Mutex.h"
#include "com/diag/hayloft/CriticalSection.h"
#include <pthread.h>

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture ThreadTest;

class MyClass {
public:
	explicit MyClass() { Logger::instance().debug("MyClass@%p: ctor\n", this); }
	virtual ~MyClass() { Logger::instance().debug("MyClass@%p: dtor\n", this); }
};

TEST_F(ThreadTest, Scope) {
	MyClass object1;
	{
		MyClass object2;
	}
}

class MyMutex : public Mutex {
private:
	int nesting;
public:
	explicit MyMutex() : nesting(0) {}
	virtual bool begin(bool block = true) {
		Logger::instance().debug("MyMutex[0x%lx]::begin: before %d\n", Thread::self(), nesting);
		bool result = Mutex::begin(block);
		Logger::instance().debug("MyMutex[0x%lx]::begin: after %d rc=%d\n", Thread::self(), nesting, result);
		++nesting;
	}
	virtual bool end() {
		--nesting;
		Logger::instance().debug("MyMutex[0x%lx]::end: before %d\n", Thread::self(), nesting);
		bool result = Mutex::end();
		Logger::instance().debug("MyMutex[0x%lx]::end: after %d rc=%d\n", Thread::self(), nesting, result);
	}
};

static MyMutex myMutex;

TEST_F(ThreadTest, Mutex) {
	ASSERT_FALSE(myMutex.isLocked());
	myMutex.begin();
	EXPECT_TRUE(myMutex.isLocked());
	myMutex.end();
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, MutexRecursive) {
	ASSERT_FALSE(myMutex.isLocked());
	myMutex.begin();
	EXPECT_TRUE(myMutex.isLocked());
	myMutex.begin();
	EXPECT_TRUE(myMutex.isLocked());
	myMutex.end();
	EXPECT_TRUE(myMutex.isLocked());
	myMutex.end();
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, CriticalSection) {
	ASSERT_FALSE(myMutex.isLocked());
	{
		CriticalSection guard(myMutex);
		EXPECT_TRUE(myMutex.isLocked());
	}
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, CriticalSectionRecursive) {
	ASSERT_FALSE(myMutex.isLocked());
	{
		CriticalSection guard1(myMutex);
		EXPECT_TRUE(myMutex.isLocked());
		{
			CriticalSection guard2(myMutex);
			EXPECT_TRUE(myMutex.isLocked());
		}
	}
	EXPECT_FALSE(myMutex.isLocked());
}

class ThreadOne : public Thread {
public:
	int * variable;
	explicit ThreadOne(int & shared) : variable(&shared) { *variable = 1; }
	virtual void run() { CriticalSection guard(myMutex); *variable = 3; }
};

TEST_F(ThreadTest, Idle) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	ThreadOne thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 1);
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, Join) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadOne thread(variable);
	EXPECT_EQ(variable, 1);
	{
		CriticalSection guard(myMutex);
		variable = 2;
		EXPECT_EQ(variable, 2);
		EXPECT_EQ(thread.start(), 0);
		EXPECT_NE(thread.start(), 0);
		EXPECT_EQ(variable, 2);
		platform.yield(platform.frequency());
		EXPECT_EQ(variable, 2);
	}
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 4);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

class ThreadTwo : public Thread {
public:
	int * variable;
	explicit ThreadTwo(int & shared) : variable(&shared) { *variable = 1; }
	virtual void run() { while (true) { *variable = 3;  cancellable(); yield(); } }
};

TEST_F(ThreadTest, Cancel) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadTwo thread(variable);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

class ThreadThree : public Thread {
public:
	int * variable;
	explicit ThreadThree(int & shared) : variable(&shared) { *variable = 1; }
	virtual void run() { CriticalSection guard(myMutex); while (true) { *variable = 3; cancellable(); if (cancelled()) { break; } yield(); } }
};

TEST_F(ThreadTest, MutexCancel) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadThree thread(variable);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_TRUE(myMutex.isLocked());
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

class ThreadFour : public Thread {
public:
	virtual void run() { CriticalSection guard(myMutex); exit(); }
};

TEST_F(ThreadTest, Exit) {
	ASSERT_FALSE(myMutex.isLocked());
	ThreadFour thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, ReturnJoin) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.join(), 0);
}

class ThreadFive : public Thread {
public:
	virtual void run() { pthread_exit(0); }
};

TEST_F(ThreadTest, ExitJoin) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.join(), 0);
}

class ThreadSix : public Thread {
public:
	virtual void run() { ::com::diag::desperado::Platform::instance().yield(::com::diag::desperado::Platform::instance().frequency()); }
};

TEST_F(ThreadTest, JoinReturn) {
	ThreadSix thread;
	EXPECT_EQ(thread.start(), 0);
	EXPECT_EQ(thread.join(), 0);
}

class ThreadSeven : public Thread {
public:
	virtual void run() { CriticalSection guard(myMutex); while (true) { cancellable(); yield(); } }
};

TEST_F(ThreadTest, MutexCancelleable) {
	ASSERT_FALSE(myMutex.isLocked());
	ThreadSeven thread;
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_TRUE(myMutex.isLocked());
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.join(), 0);
	platform.yield(platform.frequency());
	EXPECT_FALSE(myMutex.isLocked());
}

}
}
}

#endif
