/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETHEAD_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETHEAD_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Region.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketHead : public Bucket {

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	char constraint[Region::LENGTH + 1];

	::S3ResponseHandler handler;

public:

	explicit BucketHead(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit BucketHead(
		const char * bucketname,
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~BucketHead();

	virtual void start();

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
