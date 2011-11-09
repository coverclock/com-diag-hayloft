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
 */

#include "com/diag/hayloft/Logger.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class UniversalResourceIdentifierStyle {

private:

	::S3UriStyle style;

public:

	explicit UniversalResourceIdentifierStyle(::S3UriStyle uristyle = ::S3UriStyleVirtualHost)
	: style(uristyle)
	{
		Logger::instance().debug("UniversalResourceIdentifierStyle@%p: uristyle=%d\n", this, style);
	}

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
