/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_CONTEXT_
#define _H_COM_DIAG_HAYLOFT_CONTEXT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Credentials.h"
#include "com/diag/hayloft/Region.h"
#include "com/diag/hayloft/Protocol.h"
#include "com/diag/hayloft/Style.h"
#include "com/diag/hayloft/Access.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Context specifies the context within which an Action may be taken or a
 * Service may be invoked. A Context includes Credentials, Region, Protocol,
 * Style, and Access.
 */
class Context {

protected:

	std::string id;

	std::string secret;

	std::string region;

	::S3Protocol protocol;

	::S3UriStyle style;

	::S3CannedAcl access;

public:

	/**
	 * Ctor.
	 *
	 * @param cr refers to set of Credentials. This reference is only used
	 *        during construction.
	 * @param re refers to a Region. This reference is only used during
	 *        construction.
	 * @param pr refers to a Protocol. This reference is only used during
	 *        construction.
	 * @param st refers to a Style. This reference is only used during
	 *        construction.
	 * @param ac refers to an Access. This reference is only used during
	 *        construction.
	 */
	explicit Context(
		const Credentials & cr = Credentials(),
		const Region & re = Region(),
		const Protocol & pr = Protocol(),
		const Style & st = Style(),
		const Access & ac = Access()
	);

	/**
	 * Dtor.
	 */
	virtual ~Context() {}

	/**
	 * Get the access key ID.
	 *
	 * @return the access key ID.
	 */
	const char * getId() const { return id.c_str(); }

	/**
	 * Get the secret access key.
	 *
	 * @return the secret access key.
	 */
	const char * getSecret() const { return secret.c_str(); }

	/**
	 * Get the region.
	 *
	 * @return the region.
	 */
	const char * getRegion() const { return region.c_str(); }

	/**
	 * Get the protocol.
	 *
	 * @return the protocol.
	 */
	::S3Protocol getProtocol() const { return protocol; }

	/**
	 * Get the style.
	 *
	 * @return the style.
	 */
	::S3UriStyle getStyle() const { return style; }

	/**
	 * Get the access.
	 *
	 * @return the access.
	 */
	::S3CannedAcl getAccess() const { return access; }

	/**
	 * Set the Credentials.
	 *
	 * @param cr refers to a set of Credentials. This reference is not used after
	 *        this settor returns.
	 * @return a reference to this object.
	 */
	Context & setCredentials(const Credentials & cr = Credentials()) { id = cr.getId(); secret = cr.getSecret(); return *this; }

	/**
	 * Set the Region.
	 *
	 * @param re refers to a Region. This reference is not used after this
	 *        settor returns.
	 * @return a reference to this object.
	 */
	Context & setRegion(const Region & re = Region()) { region = re.getRegion(); return *this; }

	/**
	 * Set the Protocol.
	 *
	 * @param pr refers to a Protocol. This reference is not used after this
	 *        settor returns.
	 * @return a reference to this object.
	 */
	Context & setProtocol(const Protocol & pr = Protocol()) { protocol = pr.getProtocol(); return *this; }

	/**
	 * Set the Style.
	 *
	 * @param st refers to a Style. This reference is not used after this
	 *        settor returns.
	 * @return a reference to this object.
	 */
	Context & setStyle(const Style & st = Style()) { style = st.getStyle(); return *this; }

	/**
	 * Set the Access.
	 *
	 * @param ac refers to an Access. This reference is not used after this
	 *        settor returns.
	 * @return a reference to this object.
	 */
	Context & setAccess(const Access & ac = Access()) { access = ac.getAccess(); return *this; }

};

}
}
}

#endif
