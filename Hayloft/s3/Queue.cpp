/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 */

#include "com/diag/hayloft/s3/Queue.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"
#include <curl/curl.h>
#include "request_context.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Queue::Queue()
: status(::S3_create_request_context(&context))
{
	Logger::instance().debug("Queue@%p: context=%p\n", this, context);
	if (status != S3StatusOK) {
		Logger::instance().error("Queue@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Queue::~Queue() {
	if (status == S3StatusOK) {
		S3_destroy_request_context(context);
	}
}

}
}
}
}
