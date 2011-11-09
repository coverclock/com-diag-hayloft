/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Queue.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/errno.h"
#include <curl/curl.h>
#include "request_context.h"
#include <sys/select.h>

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

int Queue::pendings = 0;

Queue::Queue()
: status(::S3_create_request_context(&requests))
{
	Logger::instance().debug("Queue@%p: requests=%p\n", this, requests);
	if (status != S3StatusOK) {
		Logger::instance().error("Queue@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Queue::~Queue() {
	if (status == S3StatusOK) {
		S3_destroy_request_context(requests);
	}
}

bool Queue::all() {
	status = ::S3_runall_request_context(requests);
	return (status == ::S3StatusOK);
}

bool Queue::once(int & pending) {
	status = ::S3_runonce_request_context(requests, &pending);
	return (status == ::S3StatusOK);
}

bool Queue::ready(Milliseconds timeout) {
	bool result = false;
	do {
		fd_set reads;
		FD_ZERO(&reads);
		fd_set writes;
		FD_ZERO(&writes);
		fd_set exceptions;
		FD_ZERO(&exceptions);
		int maxfd = -1;
		status = ::S3_get_request_context_fdsets(requests, &reads, &writes, &exceptions, &maxfd);
		if (status != ::S3StatusOK) {
			Logger::instance().error("Queue@%p: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
			break;
		}
		if (maxfd < 0) { break; }
		Milliseconds maximum = ::S3_get_request_context_timeout(requests);
		if (timeout > maximum) { timeout = maximum; }
		struct timeval timeoutval = { timeout / 1000, (timeout % 1000) * 1000 };
		int rc = ::select(maxfd, &reads, &writes, &exceptions, &timeoutval);
		if (rc < 0) {
			Logger::instance().error("Queue@%p: select failed! error=%d=\"%s\"\n", this, errno, ::strerror(errno));
			break;
		}
		if (rc == 0) { break; }
		result = true;
	} while (false);
	return result;
}

}
}
}
}
