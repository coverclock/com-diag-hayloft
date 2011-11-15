/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_STYLE
#define _H_COM_DIAG_HAYLOFT_S3_STYLE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>z
 * Virtual Host:	PROTOCOL://BUCKET.s3.amazonaws.com/OBJECT
 * Path:			PROTOCOL://s3.amazonaws.com/BUCKET/OBJECT
 *
 * Using a VIRTUAL HOST style URI with mixed case bucket names yields an
 * unhelpful (to me anyway) message about keys not matching when doing a
 * BucketTest for a bucket that does not exist. Using a mixed case bucket
 * name with a PATH style URI at least says the bucket does not exist. In
 * any case, the DevGuide indicates that one should use all lower case bucket
 * names.
 */

#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Style {

private:

	::S3UriStyle style;

public:

	static const ::S3UriStyle DEFAULT = ::S3UriStyleVirtualHost;

	explicit Style(
		::S3UriStyle st = DEFAULT
	);

	virtual ~Style() {}

	::S3UriStyle getStyle() const { return style; }

};

class StyleVirtualHost : public Style {

public:

	explicit StyleVirtualHost()
	: Style(::S3UriStyleVirtualHost)
	{}

	virtual ~StyleVirtualHost() {}

};

class StylePath : public Style {

public:

	explicit StylePath()
	: Style(::S3UriStylePath)
	{}

	virtual ~StylePath() {}

};

}
}
}
}

#endif
