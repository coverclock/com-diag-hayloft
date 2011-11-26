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
#include "com/diag/hayloft/s3/Service.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Container : public Service {

protected:

	std::string canonical;

	::S3UriStyle style;

	::S3BucketContext context;

public:

	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * canonicalBucketName,
		::S3Protocol httpProtocol,
		::S3UriStyle uristyle
	);

	explicit Container(
		const char * accessKeyId,
		const char * secretAccessKey,
		const char * endPoint,
		const char * canonicalBucketName,
		::S3Protocol httpProtocol,
		::S3UriStyle uristyle,
		Multiplex & multiplex
	);

	virtual ~Container();

	const char * getCanonical() const { return canonical.c_str(); }

	::S3UriStyle getStyle() const { return style; }

private:

	void initialize();

};

}
}
}
}

#endif
