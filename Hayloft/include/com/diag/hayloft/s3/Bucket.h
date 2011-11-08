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

#include <string>
#include "com/diag/hayloft/s3/Context.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket {

public:

	explicit Bucket(const char * bucketName);

	virtual ~Bucket();

	virtual bool valid(Context & context);

	virtual void test(Context & context);

	virtual void create(Context & context);

	virtual void remove(Context & context);

private:

	std::string name;

public:

	const char * getBucketName() const { return name.c_str(); }

};

}
}
}
}

#endif
