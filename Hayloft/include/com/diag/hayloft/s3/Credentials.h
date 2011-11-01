/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CREDENTIALS
#define _H_COM_DIAG_HAYLOFT_S3_CREDENTIALS

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 * This header file defines a C++ wrapper around Bryan Ischo's C-based
 * libs3 package just to make it easier to incorporate into existing C++
 * code bases and to define defaults and implement details specific to
 * Hayloft. C++ methods are placed in the ::com::diag::hayloft::s3 namespace.
 * The original libs3 C definitions are left in the global :: namespace.<BR>
 */

#include <string>
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Credentials {

public:

	static const char * ACCESS_KEY_ID_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_ACCESS_KEY_ID"; }

	static const size_t ACCESS_KEY_ID_LEN = 20;

	static const char * SECRET_ACCESS_KEY_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_SECRET_ACCESS_KEY"; }

	static const size_t SECRET_ACCESS_KEY_LEN = 40;

	explicit Credentials(const char * accessKeyId = 0, const char * secretAccessKey = 0);

	virtual ~Credentials();

protected:

	std::string id;

	std::string secret;

public:

	bool successful() const { return ((id.length() == ACCESS_KEY_ID_LEN) && (secret.length() == SECRET_ACCESS_KEY_LEN)); }

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	Credentials(const Credentials& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	Credentials& operator=(const Credentials& that);

};

}
}
}
}

#endif