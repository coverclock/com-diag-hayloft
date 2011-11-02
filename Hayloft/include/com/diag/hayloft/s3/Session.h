/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SESSION
#define _H_COM_DIAG_HAYLOFT_S3_SESSION

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Session {

public:

	static const char * USER_AGENT_STR() { return "s3.hayloft.diag.com"; }

	static const char * USER_AGENT_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_USER_AGENT"; }

	static const char * HOST_NAME_STR() { return S3_DEFAULT_HOSTNAME; }

	static const char * HOST_NAME_ENV() { return "COM_DIAG_HAYLOFT_S3_SESSION_HOST_NAME"; }

	explicit Session(const char * userAgentInfo = 0, int flags = S3_INIT_ALL, const char * defaultS3HostName = 0);

	virtual ~Session();

protected:

	std::string useragent;

	std::string hostname;

	::S3Status status;

public:

	operator bool() const { return (status == S3StatusOK); }

	const char * getUserAgent() const { return useragent.c_str(); }

	const char * getHostName() const { return hostname.c_str(); }

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	Session(const Session& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	Session& operator=(const Session& that);

};

}
}
}
}

#endif
