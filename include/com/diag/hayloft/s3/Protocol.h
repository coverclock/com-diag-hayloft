/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_PROTOCOL
#define _H_COM_DIAG_HAYLOFT_S3_PROTOCOL

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

class Protocol {

private:

	::S3Protocol protocol;

public:

	static const ::S3Protocol DEFAULT = ::S3ProtocolHTTPS;

	explicit Protocol(
		::S3Protocol pr = DEFAULT
	);

	virtual ~Protocol() {}

	::S3Protocol getProtocol() const { return protocol; }

};

class ProtocolSecure : public Protocol {

public:

	explicit ProtocolSecure()
	: Protocol(::S3ProtocolHTTPS)
	{}

	virtual ~ProtocolSecure() {}

};

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
