/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/ready.h"
#include "com/diag/desperado/Platform.h"
#include <sys/select.h>

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Multiplex::Multiplex()
: status(::S3_create_request_context(&requests))
{
	initialize();
}

Multiplex::~Multiplex() {
	if (requests != 0) {
		S3_destroy_request_context(requests);
	}
}

void Multiplex::initialize() {
	if (status != S3StatusOK) {
		Logger::instance().error("Multiplex@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, tostring(status));
	}
}

}
}
}
}
