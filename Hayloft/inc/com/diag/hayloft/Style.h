/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_STYLE_
#define _H_COM_DIAG_HAYLOFT_STYLE_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>z
 */

#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Style specifies the style of Uniform Resource Identifier, Virtual Host or
 * Path, that should be used. Most applications will use the Virtual Host style,
 * although that requires bucket names to conform to the rules of the Domain
 * Name System (DNS).
 *
 * Virtual Host:	PROTOCOL://BUCKET.s3.amazonaws.com/OBJECT
 *
 * Path:			PROTOCOL://s3.amazonaws.com/BUCKET/OBJECT
 */
class Style {

protected:

	::S3UriStyle style;

public:

	/**
	 * This is the default style.
	 *
	 * Using a VIRTUAL HOST style URI with mixed case bucket names yields an
	 * unhelpful (to me anyway) message about keys not matching when doing a
	 * BucketTest for a bucket that does not exist. Using a mixed case bucket
	 * name with a PATH style URI at least says the bucket does not exist. In
	 * any case, the DevGuide indicates that one should use all lower case
	 * bucket names.
	 */
	static const ::S3UriStyle DEFAULT = ::S3UriStyleVirtualHost;

	/**
	 * Ctor.
	 *
	 * @param st is the style to use.
	 */
	explicit Style(
		::S3UriStyle st = DEFAULT
	);

	/**
	 * Dtor.
	 */
	virtual ~Style() {}

	/**
	 * Get the style.
	 *
	 * @return the style.
	 */
	::S3UriStyle getStyle() const { return style; }

	/**
	 * Set the style.
	 *
	 * @param st is the style to use.
	 * @return a reference to this object.
	 */
	Style & setStyle(::S3UriStyle st = DEFAULT) { style = st; return *this; }

};

/**
 * StyleVirtualHost specifies a Virtual Host style of URI.
 */
class StyleVirtualHost : public Style {

public:

	explicit StyleVirtualHost()
	: Style(::S3UriStyleVirtualHost)
	{}

	virtual ~StyleVirtualHost() {}

};

/**
 * StylePath specifies a Path style of URI.
 */
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

#endif
