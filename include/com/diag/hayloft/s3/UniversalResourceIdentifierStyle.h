/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_UNIVERSALRESOURCEIDENTIFIERSTYLE
#define _H_COM_DIAG_HAYLOFT_S3_UNIVERSALRESOURCEIDENTIFIERSTYLE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 *
 * Using a Virtual Host style URI causes libs3 to construct GET requests
 * that appear to require DNS support for your bucket name. This results in
 * an error return from S3 with the message  "The request signature we
 * calculated does not match the signature you provided. Check your key and
 * signing method."
 *
 */

#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class UniversalResourceIdentifierStyle {

private:

	::S3UriStyle style;

public:

	static const ::S3UriStyle DEFAULT = ::S3UriStylePath;

	explicit UniversalResourceIdentifierStyle(::S3UriStyle uri = DEFAULT);

	virtual ~UniversalResourceIdentifierStyle() {}

	::S3UriStyle getStyle() const { return style; }

};

class UniversalResourceIdentifierStyleVirtualHost : public UniversalResourceIdentifierStyle {

public:

	explicit UniversalResourceIdentifierStyleVirtualHost()
	: UniversalResourceIdentifierStyle(::S3UriStyleVirtualHost)
	{}

	virtual ~UniversalResourceIdentifierStyleVirtualHost() {}

};

class UniversalResourceIdentifierStylePath : public UniversalResourceIdentifierStyle {

public:

	explicit UniversalResourceIdentifierStylePath()
	: UniversalResourceIdentifierStyle(::S3UriStylePath)
	{}

	virtual ~UniversalResourceIdentifierStylePath() {}

};

}
}
}
}

#endif
