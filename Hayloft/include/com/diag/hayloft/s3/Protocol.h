/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_PROTOCOL_
#define _H_COM_DIAG_HAYLOFT_S3_PROTOCOL_

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

/**
 * Protocol specifies which protocol supported by libs3 (and hence Hayloft) to
 * use: HTTP (Unsecure) or HTTPS (Secure).
 */
class Protocol {

public:

	/**
	 * This is the default protocol.
	 */
	static const ::S3Protocol DEFAULT = ::S3ProtocolHTTPS;

protected:

	::S3Protocol protocol;

public:

	/**
	 * Ctor.
	 *
	 * @param pr is the protocol to use.
	 */
	explicit Protocol(
		::S3Protocol pr = DEFAULT
	);

	/**
	 * Dtor.
	 */
	virtual ~Protocol() {}

	/**
	 * Get the protocol.
	 *
	 * @return the protocol.
	 */
	::S3Protocol getProtocol() const { return protocol; }

	/**
	 * Set the protocol.
	 *
	 * @param pr is the protocol to use.
	 * @return a reference to this object.
	 */
	Protocol & setProtocol(::S3Protocol pr = DEFAULT) { protocol = pr; return *this; }

};

/**
 * ProtocolSecure is a Protocol that uses HTTPS.
 */
class ProtocolSecure : public Protocol {

public:

	explicit ProtocolSecure()
	: Protocol(::S3ProtocolHTTPS)
	{}

	virtual ~ProtocolSecure() {}

};

/**
 * ProtocolUnsecure is a Protocol that uses HTTP.
 */
class ProtocolUnsecure : public Protocol {

public:

	explicit ProtocolUnsecure()
	: Protocol(::S3ProtocolHTTP)
	{}

	virtual ~ProtocolUnsecure() {}

};

}
}
}
}

#endif
