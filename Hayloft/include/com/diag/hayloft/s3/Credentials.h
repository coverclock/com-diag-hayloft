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
 */

#include <string>
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace desperado {
class Input;
}
}
}

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Credentials {

private:

	std::string id;

	std::string secret;

public:

	static const char * ACCESS_KEY_ID_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_ACCESS_KEY_ID"; }

	static const size_t ACCESS_KEY_ID_LEN = 20;

	static const char * SECRET_ACCESS_KEY_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_SECRET_ACCESS_KEY"; }

	static const size_t SECRET_ACCESS_KEY_LEN = 40;

	static const char * obfuscate(const char * str);

	explicit Credentials(const char * accessKeyId = 0, const char * secretAccessKey = 0);

	explicit Credentials(::com::diag::desperado::Input & accessKeyIdIn, ::com::diag::desperado::Input & secretAccessKeyIn);

	explicit Credentials(::com::diag::desperado::Input * accessKeyIdInPtr /* TAKEN */, ::com::diag::desperado::Input * secretAccessKeyInPtr /* TAKEN */);

	virtual ~Credentials();

	operator bool() const { return ((id.length() == ACCESS_KEY_ID_LEN) && (secret.length() == SECRET_ACCESS_KEY_LEN)); }

	const char * getId() const { return id.c_str(); }

	const char * getSecret() const { return secret.c_str(); }

protected:

	bool audit();

};

}
}
}
}

#endif
