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

int Multiplex::dontcare = 0;

Multiplex::Multiplex()
: Plex()
, taken(0)
, status(::S3_create_request_context(&taken))
{
	requests = taken;
	initialize();
}

Multiplex::Multiplex(::S3RequestContext * untaken)
: Plex(untaken)
, taken(0)
, status(::S3StatusOK)
{
	initialize();
}

Multiplex::~Multiplex() {
	if (taken != 0) {
		S3_destroy_request_context(taken);
	}
}

void Multiplex::initialize() {
	if (status != S3StatusOK) {
		Logger::instance().error("Multiplex@%p: S3_create_request_context failed! status=%d=\"%s\"\n", this, status, tostring(status));
	}
}

int Multiplex::complete() {
	int rc = 0;
	::S3Status status = ::S3_runall_request_context(requests);
	if (status != S3StatusOK) {
		Logger::instance().error("Multiplex@%p: S3_runall_request_context failed! status=%d=\"%s\"\n", this, status, tostring(status));
		rc = (::S3_status_is_retryable(status) != 0) ? RETRY : ERROR;
	}
	return rc;
}

int Multiplex::iterate(int & pending /* static dontcare */) {
	Logger & logger = Logger::instance();
	int rc = 0;
	int count = 0;
	::S3Status status = ::S3_runonce_request_context(requests, &count);
	if (status != S3StatusOK) {
		logger.error("Multiplex@%p: S3_runonce_request_context failed! status=%d=\"%s\"\n", this, status, tostring(status));
		rc |= (::S3_status_is_retryable(status) != 0) ? RETRY : ERROR;
	}
	if (count > 0) {
		logger.debug("Multiplex@%p: pending=%d\n", this, count);
		rc |= PENDING;
	}
	pending = count;
	return rc;
}

int Multiplex::ready(Milliseconds timeout) {
	Logger & logger = Logger::instance();
	int rc = 0;
	do {
		// We really only care about the reads, but we provide all three fd_sets
		// all zeroed out as specified, even though we won't use two of them.
		fd_set reads;
		FD_ZERO(&reads);
		fd_set writes;
		FD_ZERO(&writes);
		fd_set exceptions;
		FD_ZERO(&exceptions);
		int maxfd = -1;
		::S3Status status = ::S3_get_request_context_fdsets(requests, &reads, &writes, &exceptions, &maxfd);
		if (status != ::S3StatusOK) {
			logger.error("Multiplex@%p: S3_get_request_context_fdsets failed! status=%d=\"%s\"\n", this, status, tostring(status));
			rc |= (::S3_status_is_retryable(status) != 0) ? RETRY : ERROR;
			break;
		}
		logger.debug("Multiplex@%p: maxfd=%d\n", this, maxfd);
		if (maxfd < 0) {
			break;
		}
		Milliseconds maximum = ::S3_get_request_context_timeout(requests);
		if (timeout > maximum) { timeout = maximum; }
		if (timeout < 0) { timeout = DEFAULT; } // ::S3_get_request_context_timeout() can return -1 indicating no suggested value.
		logger.debug("Multiplex@%p: milliseconds=%lld\n", this, timeout);
		struct timeval timeoutval = { timeout / 1000, (timeout % 1000) * 1000 };
		// No really, we only care about the reads. We assume if libs3 had
		// something to write it would do so. The fact that sockets are
		// writable without blocking isn't really useful to know.
		if (::select(maxfd, &reads, 0, 0, &timeoutval) < 0) {
			logger.error("Multiplex@%p: select failed! error=%d=\"%s\"\n", this, errno, ::strerror(errno));
			rc |= ERROR;
			break;
		}
		// This is really inefficient but it is the only reliable way to do it
		// without cracking open the fd_set structure and relying on its
		// specific implementation.
		for (int fd = 0; fd <= maxfd; ++fd) {
			if (FD_ISSET(fd, &reads)) { rc |= READY; break; }
		}
	} while (false);
	logger.debug("Multiplex@%p: ready=0x%x\n", this, rc);
	return rc;
}

int Multiplex::service(Milliseconds timeout, int limit) {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	int rc;
	for (int ii = 0; ii < limit; ++ii) {
		rc = 0;
		// You must call iterate() first if you use request contexts in libs3.
		// In that case, libs3 doesn't do any socket writes initially. So you
		// must kick it off by doing at least one iteration, in which it will
		// do all of the socket writes it needs for each pending Action, after
		// which it's all a matter of waiting for S3 to respond and driving the
		// underlying state machine as sockets become READY.
		rc |= iterate();
		if ((rc & ERROR) != 0) { break; }
		if ((rc & RETRY) != 0) { platform.yield(); continue; }
		if ((rc & PENDING) == 0) { break; }
		rc |= ready(timeout);
		if ((rc & ERROR) != 0) { break; }
		if ((rc & RETRY) != 0) { platform.yield(); continue; }
		if ((rc & READY) == 0) { break; }
	}
	Logger::instance().debug("Multiplex@%p: service=0x%x\n", this, rc);
	return rc;
}

}
}
}
}
