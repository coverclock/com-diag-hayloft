/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ACTION_
#define _H_COM_DIAG_HAYLOFT_S3_ACTION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Multiplex;

class Action {

public:

	static const int BUSY = intmaxof(::S3Status);

	static const int IDLE = intmaxof(::S3Status) - 1;

private:

	static ::S3Status responsePropertiesCallback(const ::S3ResponseProperties * properties, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	::S3RequestContext * requests;

	::S3Status status;

	::S3ResponseHandler handler;

	::S3Status state() const { ::com::diag::desperado::MemoryBarrier barrier; return status; }

public:

	explicit Action();

	explicit Action(Multiplex & multiplex);

	virtual ~Action();

	operator bool() const { ::S3Status temporary = state(); return ((temporary != IDLE) && (temporary != BUSY)); }

	bool isIdle() const { return (state() == IDLE); }

	bool isBusy() const { return (state() == BUSY); }

	bool isRetryable() const { return (::S3_status_is_retryable(state()) != 0); }

	bool isSuccessful() const { return (state() == ::S3StatusOK); }

	::S3Status getStatus(const char ** description = 0) const;

	virtual void start();

protected:

	virtual ::S3Status properties(const ::S3ResponseProperties * properties);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

};

}
}
}
}

#endif
