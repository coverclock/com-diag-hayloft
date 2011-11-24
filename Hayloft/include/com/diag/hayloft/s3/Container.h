/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CONTAINER_
#define _H_COM_DIAG_HAYLOFT_S3_CONTAINER_

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

class Container : public Action {

protected:

	std::string id;

	std::string secret;

	std::string endpoint;

	std::string name;

	::S3Protocol protocol;

	::S3UriStyle style;

	::S3BucketContext context;

public:

	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * bucketName,
		::S3Protocol proto,
		::S3UriStyle uristyle
	);

	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * bucketName,
		::S3Protocol proto,
		::S3UriStyle uristyle,
		Multiplex & multiplex
	);

	virtual ~Container();

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

	const char * getEndpoint() const { return endpoint.c_str(); }

	const char * getName() const { return name.c_str(); }

	::S3Protocol getProtocol() const { return protocol; }

	::S3UriStyle getStyle() const { return style; }

private:

	void initialize();

};

}
}
}
}

#endif
