/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_GRANTXML_
#define _H_COM_DIAG_HAYLOFT_S3_GRANTXML_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Grant.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 *
 */
class GrantXml : public Grant {

protected:

	std::string xml;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param xmlacl is a C string containing an XML representation of an S3 ACL.
	 */
	explicit GrantXml(
		const char * xmlacl
	);

	/**
	 * Dtor.
	 */
	virtual ~GrantXml();

	const char * getXml() const { return xml.c_str(); }

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
