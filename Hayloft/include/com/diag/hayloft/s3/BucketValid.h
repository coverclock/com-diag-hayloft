/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETVALID_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Bucket.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketValid : public Bucket {

public:

	explicit BucketValid(
		const char * bucketname,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~BucketValid() {}

	operator bool() const { return (status == ::S3StatusOK); }

	bool isValid() const { return (status == ::S3StatusOK); }

private:

	void initialize();

	void execute();

};


}
}
}
}

#endif
