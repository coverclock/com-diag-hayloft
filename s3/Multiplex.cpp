/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Multiplex.h"
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

int Multiplex::dontcare = 0;

Multiplex::Multiplex()
: requests(0)
, status(::S3_create_request_context(&requests))
{
	Logger & logger = Logger::instance();
	logger.debug("Multiplex@%p: requests=%p\n", this, requests);
	if (status != S3StatusOK) {
		logger.error("Multiplex@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
}

Multiplex::~Multiplex() {
	if (status == S3StatusOK) {
		S3_destroy_request_context(requests);
	}
}

bool Multiplex::all() {
	status = ::S3_runall_request_context(requests);
	if (status != S3StatusOK) {
		Logger::instance().error("Multiplex@%p: S3_runall_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
	return (status == ::S3StatusOK);
}

bool Multiplex::once(int & pending) {
	Logger & logger = Logger::instance();
	int count = 0;
	status = ::S3_runonce_request_context(requests, &count);
	if (status != S3StatusOK) {
		logger.error("Multiplex@%p: S3_runonce_request_context failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
	}
	if (count > 0) {
		logger.debug("Multiplex@%p: pending=%d\n", this, count);
	}
	pending = count;
	return (status == ::S3StatusOK);
}

int Multiplex::ready(Milliseconds timeout) {
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
			logger.error("Multiplex@%p: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", this, status, ::S3_get_status_name(status));
			rc |= ERROR;
			if (::S3_status_is_retryable(status) != 0) {
				rc |= RETRY;
			}
			break;
		}
		logger.debug("Multiplex@%p: maxfd=%d\n", this, maxfd);
		if (maxfd < 0) {
			break;
		}
		Milliseconds maximum = ::S3_get_request_context_timeout(requests);
		if (timeout > maximum) { timeout = maximum; }
		if (timeout < 0) { timeout = DEFAULT; } // curl_multi_timeout() can return -1 indicating no suggested value.
		logger.debug("Multiplex@%p: milliseconds=%lld\n", this, timeout);
		struct timeval timeoutval = { timeout / 1000, (timeout % 1000) * 1000 };
		rc = ::select(maxfd, &reads, &writes, &exceptions, &timeoutval);
		if (rc < 0) {
			logger.error("Multiplex@%p: select failed! error=%d=\"%s\"\n", this, errno, ::strerror(errno));
			rc |= ERROR;
			break;
		}
		fd_set zeros;
		FD_ZERO (&zeros);
		if (std::memcmp(&reads, &zeros, sizeof(fd_set)) == 0) {
			rc |= READ;
		}
		if (std::memcmp(&writes, &zeros, sizeof(fd_set)) == 0) {
			rc |= WRITE;
		}
		if (std::memcmp(&exceptions, &zeros, sizeof(fd_set)) == 0) {
			rc |= EXCEPTION;
		}
	} while (false);
	logger.debug("Multiplex@%p: ready=0x%x\n", this, rc);
	return rc;
}

int Multiplex::service(Milliseconds timeout, int limit) {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	int rc;
	int rc2;
	int pending;
	do {
		rc = 0;
		pending = 0;
		if (once(pending)) {
			// Do nothing.
		} else if (::S3_status_is_retryable(status) != 0) {
			rc |= ERROR | RETRY;
		} else {
			rc |= ERROR;
			break;
		}
		if (pending > 0) {
			rc |= PENDING;
		}
		rc2 = ready(timeout) & (~(WRITE | EXCEPTION));
		rc |= rc2;
		if ((rc2 & ERROR) == 0) {
			// Do nothing.
		} else if ((rc2 & RETRY) != 0) {
			platform.yield();
		} else {
			break;
		}
	} while ((((rc & ERROR) == 0) || ((rc & RETRY) != 0)) && ((rc & (PENDING | READ)) != 0) && ((--limit) > 0));
	Logger::instance().debug("Multiplex@%p: service=0x%x\n", this, rc);
	return rc;
}

}
}
}
}
