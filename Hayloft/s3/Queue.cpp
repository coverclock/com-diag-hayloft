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
#include "com/diag/desperado/ready.h"
#include "com/diag/desperado/Platform.h"
#include <curl/curl.h>
#include "request_context.h"
#include <sys/select.h>

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

int Queue::dontcare = 0;

Queue::Queue()
: requests(0)
, status(::S3_create_request_context(&requests))
{
	Logger & logger = Logger::instance();
	logger.debug("Queue@%p: requests=%p\n", this, requests);
	if (status != S3StatusOK) {
		logger.error("Queue@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Queue::~Queue() {
	if (status == S3StatusOK) {
		S3_destroy_request_context(requests);
	}
}

bool Queue::all() {
	status = ::S3_runall_request_context(requests);
	if (status != S3StatusOK) {
		Logger::instance().error("Queue@%p: S3_runall_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
	return (status == ::S3StatusOK);
}

bool Queue::once(int & pending) {
	Logger & logger = Logger::instance();
	int count = 0;
	status = ::S3_runonce_request_context(requests, &count);
	if (status != S3StatusOK) {
		logger.error("Queue@%p: S3_runonce_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
	if (count > 0) {
		logger.debug("Queue@%p: pending=%d\n", this, count);
	}
	pending = count;
	return (status == ::S3StatusOK);
}

int Queue::ready(Milliseconds timeout) {
	Logger & logger = Logger::instance();
	int rc = 0;
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
			logger.error("Queue@%p: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
			rc |= DESPERADO_DESCRIPTOR_READY_ERROR;
			break;
		}
		logger.debug("Queue@%p: maxfd=%d\n", this, maxfd);
		if (maxfd < 0) {
			break;
		}
		Milliseconds maximum = ::S3_get_request_context_timeout(requests);
		if (timeout > maximum) { timeout = maximum; }
		if (timeout < 0) { timeout = DEFAULT; } // curl_multi_timeout() can return -1 indicating no suggested value.
		logger.debug("Queue@%p: milliseconds=%lld\n", this, timeout);
		struct timeval timeoutval = { timeout / 1000, (timeout % 1000) * 1000 };
		rc = ::select(maxfd, &reads, &writes, &exceptions, &timeoutval);
		if (rc < 0) {
			logger.error("Queue@%p: select failed! error=%d=\"%s\"\n", this, errno, ::strerror(errno));
			rc |= DESPERADO_DESCRIPTOR_READY_ERROR;
			break;
		}
		fd_set zeros;
		FD_ZERO (&zeros);
		if (std::memcmp(&reads, &zeros, sizeof(fd_set)) == 0) {
			rc |= DESPERADO_DESCRIPTOR_READY_READ;
		}
		if (std::memcmp(&writes, &zeros, sizeof(fd_set)) == 0) {
			rc |= DESPERADO_DESCRIPTOR_READY_WRITE;
		}
		if (std::memcmp(&exceptions, &zeros, sizeof(fd_set)) == 0) {
			rc |= DESPERADO_DESCRIPTOR_READY_EXCEPTION;
		}
	} while (false);
	logger.debug("Queue@%p: ready=0x%x\n", this, rc);
	return rc;
}

bool Queue::service(Milliseconds timeout, int limit) {
	bool backlogged;
	bool successful;
	int pending;
	int bits;
	do {
		pending = 0;
		successful = once(pending);
		bits = ready(timeout);
		backlogged = successful && (pending > 0) && ((bits & DESPERADO_DESCRIPTOR_READY_READ) != 0);
	} while (backlogged && ((--limit) > 0));
	return backlogged;
}


}
}
}
}
