/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_SERVICE_
#define _H_COM_DIAG_HAYLOFT_S3_SERVICE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Action.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Service : public Action {

protected:

	std::string id;

	std::string secret;

	std::string endpoint;

	::S3Protocol protocol;

public:

	explicit Service(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		::S3Protocol httpProtocol
	);

	explicit Service(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		::S3Protocol httpProtocol,
		Multiplex & multiplex
	);

	virtual ~Service();

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

	const char * getEndpoint() const { return endpoint.c_str(); }

	::S3Protocol getProtocol() const { return protocol; }

private:

	void initialize();

};

}
}
}
}

#endif
