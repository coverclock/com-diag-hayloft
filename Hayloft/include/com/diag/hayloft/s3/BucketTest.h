/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETTEST

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/LocationConstraint.h"
#include "com/diag/hayloft/s3/Status.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session;
class Queue;

class BucketTest {

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	::S3Status status;

	char constraint[LocationConstraint::LENGTH];

	::S3ResponseHandler handler;

public:

	explicit BucketTest(Session & session, const char * name, const Context & context = Context());

	explicit BucketTest(Session & session, const char * name, Queue & queue, const Context & context = Context());

	virtual ~BucketTest() {}

	operator bool() { return (status != BUSY); }

	const char * getLocationConstraint() { return constraint; }

protected:

	void run(Session & session, const char * name, Queue * queuep, const Context & context);

	virtual ::S3Status properties(const ::S3ResponseProperties * properties);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

};

}
}
}
}

#endif
