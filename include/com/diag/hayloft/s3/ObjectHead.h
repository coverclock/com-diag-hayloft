/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTHEAD_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTHEAD_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class ObjectHead : public Object {

public:

	explicit ObjectHead(
		const char * keyname,
		const Bucket & bucket
	);

	explicit ObjectHead(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex
	);

	virtual ~ObjectHead();

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
