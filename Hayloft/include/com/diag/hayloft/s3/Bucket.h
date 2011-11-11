/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKET
#define _H_COM_DIAG_HAYLOFT_S3_BUCKET

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Status.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session;
class Queue;

class Bucket {

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	const Session & session;

	std::string name;

	::S3RequestContext * requests;

	const Context & context;

	::S3Status status;

	::S3ResponseHandler handler;

	explicit Bucket(const Session & se, const char * na, const Context & co = Context());

	explicit Bucket(const Session & se, const char * na, Queue & qu, const Context & co = Context());

public:

	virtual ~Bucket();

	operator bool() const { return (status != BUSY); }

	bool isBusy() const { return (status == BUSY); }

	bool isRetryable() const { return (::S3_status_is_retryable(status) != 0); }

	::S3Status getStatus() const { return status; }

	const char * getName() const { return name.c_str(); }

protected:

	virtual ::S3Status properties(const ::S3ResponseProperties * properties);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

};

}
}
}
}

#endif
