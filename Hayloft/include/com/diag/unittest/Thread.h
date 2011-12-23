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
#include "com/diag/hayloft/Uncancellable.h"
#include "com/diag/hayloft/Condition.h"
#include <pthread.h>

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::hayloft;

typedef Fixture ThreadTest;

struct MyClass {
	explicit MyClass() {
		Logger::instance().debug("MyClass@%p: ctor\n", this);
	}
	virtual ~MyClass() {
		Logger::instance().debug("MyClass@%p: dtor\n", this);
	}
};

TEST_F(ThreadTest, Scope) {
	MyClass object1;
	{
		MyClass object2;
	}
}

struct MyMutex : public Mutex {
	int nesting;
	explicit MyMutex()
	: nesting(0)
	{}
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

struct ThreadWait : public Thread {
	int & variable;
	explicit ThreadWait(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		CriticalSection guard(myMutex);
		variable = 3;
		return 0;
	}
};

TEST_F(ThreadTest, Idle) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	ThreadWait thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 1);
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, Join) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadWait thread(variable);
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
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 4);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, Wait) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadWait thread(variable);
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
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 4);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

struct ThreadCancel : public Thread {
	int & variable;
	explicit ThreadCancel(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		while (true) {
			variable = 3;
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Cancel) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadCancel thread(variable);
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
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
}

struct ThreadNotify : public Thread {
	int & variable;
	explicit ThreadNotify(int & shared)
	: variable(shared) {
		variable = 1;
	}
	virtual void * run() {
		CriticalSection guard(myMutex);
		while (true) {
			variable = 3;
			if (notified()) {
				break;
			}
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Notify) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadNotify thread(variable);
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_TRUE(myMutex.isLocked());
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
	EXPECT_FALSE(myMutex.isLocked());
}

struct ThreadExit : public Thread {
	virtual void * run() {
		CriticalSection guard(myMutex);
		exit();
		return 0;
	}
};

TEST_F(ThreadTest, Exit) {
	ASSERT_FALSE(myMutex.isLocked());
	ThreadExit thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_FALSE(myMutex.isLocked());
}

TEST_F(ThreadTest, ReturnWait) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.wait(), 0);
}

TEST_F(ThreadTest, ExitWait) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.wait(), 0);
}

struct ThreadWaitReturn : public Thread {
	virtual void * run() {
		::com::diag::desperado::Platform::instance().yield(::com::diag::desperado::Platform::instance().frequency());
		return 0;
	}
};

TEST_F(ThreadTest, WaitReturn) {
	ThreadWaitReturn thread;
	EXPECT_EQ(thread.start(), 0);
	EXPECT_EQ(thread.wait(), 0);
}

static MyMutex badMutex;

struct MyCriticalSection {
    Mutex & mutex;
    explicit MyCriticalSection(Mutex & rm)
    : mutex(rm)
    {
    	mutex.begin(false);
    }
    virtual ~MyCriticalSection() {
    	mutex.end();
    }
};

struct ThreadLeavesMutexLocked : public Thread {
	virtual void * run() {
		MyCriticalSection guard(badMutex);
		while (true) {
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, CancelLeavesMutexLocked) {
	ASSERT_FALSE(badMutex.isLocked());
	ThreadLeavesMutexLocked thread;
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_TRUE(badMutex.isLocked());
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(thread.join(), 0);
	if (badMutex.isLocked()) {
		logger.print("Stack was NOT unwound upon thread cancel!\n");
	} else {
		logger.print("Stack WAS unwound upon thread cancel!\n");
	}
}

struct ThreadInstance : public Thread {
	Thread ** here;
	pthread_t * id;
	explicit ThreadInstance(Thread ** hh, pthread_t * ii)
	: here(hh)
	, id(ii)
	{}
	virtual void * run() {
		*here = &Thread::instance();
		*id = Thread::self();
		return 0;
	}
};

TEST_F(ThreadTest, Instance) {
	EXPECT_EQ(pthread_self(), Thread::self());
	EXPECT_EQ(pthread_self(), Thread::instance().getIdentity());
	Thread * here = 0;
	pthread_t id = 0;
	ThreadInstance thread(&here, &id);
	EXPECT_EQ(thread.start(), 0);
	EXPECT_EQ(thread.wait(), 0);
	ASSERT_NE(here, (Thread*)0);
	EXPECT_EQ(here, &thread);
	EXPECT_EQ(id, thread.getIdentity());
}

static Mutex conditionmutex;
static Condition conditionodd;
static Condition conditioneven;

struct ThreadCondition : public Thread {
	int & variable;
	explicit ThreadCondition(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		CriticalSection guard(conditionmutex);
		while (variable < 100) {
			Logger::instance().debug("Thread: before %d\n", variable);
			while ((variable % 2) == 0) {
				conditionodd.wait(conditionmutex);
			}
			Logger::instance().debug("Thread: after %d\n", variable);
			++variable;
			conditioneven.signal();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Condition) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadCondition thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.start(), 0);
	Thread::yield();
	EXPECT_EQ(variable, 1);
	{
		CriticalSection guard(conditionmutex);
		while (variable < 99) {
			Logger::instance().debug("Main: before %d\n", variable);
			while ((variable % 2) != 0) {
				conditioneven.wait(conditionmutex);
			}
			Logger::instance().debug("Main: after %d\n", variable);
			++variable;
			conditionodd.signal();
		}
	}
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 100);
}

struct ThreadUncancellable : public Thread {
	int & variable;
	explicit ThreadUncancellable(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		Uncancellable sentry;
		MyCriticalSection guard(myMutex);
		while (!notified()) {
			++variable;
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Uncancellable) {
	ASSERT_FALSE(myMutex.isLocked());
	int variable = 1;
	ThreadUncancellable thread(variable);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_TRUE(myMutex.isLocked());
	EXPECT_FALSE(thread.cancelled());
	int one = variable;
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	platform.yield(platform.frequency());
	int two = variable;
	EXPECT_NE(one, two);
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_FALSE(myMutex.isLocked());
}

static void * function(void * context) {
	*((int *)context) = 1;
	return context;
}

TEST_F(ThreadTest, Function) {
	int variable = 0;
	Thread thread;
	EXPECT_EQ(variable, 0);
	EXPECT_EQ(thread.start(function, &variable), 0);
	EXPECT_EQ(thread.wait(), 0);
	EXPECT_EQ(variable, 1);
	void * result = 0;
	EXPECT_EQ(thread.join(result), 0);
	EXPECT_NE(result, (void *)0);
	EXPECT_EQ(result, &variable);
}

}
}
}

#endif
