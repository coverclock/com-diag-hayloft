/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_CREDENTIALS_
#define _H_COM_DIAG_HAYLOFT_S3_CREDENTIALS_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Credentials incorporate the access key ID (sort of a login or account number)
 * and secret access key (a password or encryption key) that AWS provides to you
 * so that you may securely identify yourself to them when taking Actions or
 * accessing Services. It is important that the twenty-character access key ID
 * and the forty-character encryption key remain secret. Unless this translation
 * unit is compiled with a special option, the access key ID and secret access
 * key are obfuscated in any log messages. Your credentials can be explicitly
 * specified as arguments to the constructor, or taken from environmental
 * variables.
 */
class Credentials {

public:

	/**
	 * This is the name of the environmental variable that may specify your
	 * twenty-character access key ID.
	 */
	static const char * ACCESS_KEY_ID_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_ACCESSKEYID"; }

	/**
	 * This is the name of the environmental variable that may specify your
	 * forty-character secret access key.
	 */
	static const char * SECRET_ACCESS_KEY_ENV() { return "COM_DIAG_HAYLOFT_S3_CREDENTIALS_SECRETACCESSKEY"; }

	/**
	 * This is the fixed length of the access key ID.
	 */
	static const size_t ACCESS_KEY_ID_LEN = 20;

	/**
	 * This is the fixed length of the secret access key.
	 */
	static const size_t SECRET_ACCESS_KEY_LEN = 40;

	/**
	 * This function generates a obfuscated string of the same length as its
	 * input string unless a special compile option is used.
	 *
	 * @param str is the input string.
	 * @return an obfuscated string or (specially) the input string.
	 */
	static const char * obfuscate(const char * str);

protected:

	std::string id;

	std::string secret;

public:

	/**
	 * Ctor.
	 *
	 * @param accessKeyId is the access key ID or if null it is taken from the
	 *        environment. A copy is made of this C string.
	 * @param secretAccessKey is the secret access key or if null it is taken
	 *        from the environment. A copy is made of this C string.
	 */
	explicit Credentials(
		const char * accessKeyId = 0,
		const char * secretAccessKey = 0
	);

	/**
	 * Dtor.
	 */
	virtual ~Credentials();

	/**
	 * Returns true if the access key ID and the secret access key are the
	 * expected lengths.
	 *
	 * @return true if the access key ID and secret access key are the expected
	 * lengths.
	 */
	operator bool() const { return ((id.length() == ACCESS_KEY_ID_LEN) && (secret.length() == SECRET_ACCESS_KEY_LEN)); }

	/**
	 * Get the access key ID.
	 *
	 * @return the access key ID.
	 */
	const char * getId() const { return id.c_str(); }

	/**
	 * Get the secret access key ID.
	 *
	 * @return the secret access key ID.
	 */
	const char * getSecret() const { return secret.c_str(); }

	/**
	 * Set the access key ID.
	 *
	 * @param accessKeyId is the access key ID or if null it is taken from the
	 *        environment. A copy is made of this C string.
	 */
	Credentials & setId(const char * accessKeyId = 0) { id = set(accessKeyId, ACCESS_KEY_ID_ENV(), ""); return *this; }

	/**
	 * Set the secret access key.
	 *
	 * @param secretAccessKey is the secret access key or if null it is taken
	 *        from the environment. A copy is made of this C string.
	 */
	Credentials & setSecret(const char * secretAccessKey = 0) { secret = set(secretAccessKey, SECRET_ACCESS_KEY_ENV(), ""); return *this; }

protected:

	void initialize();

};

}
}
}
}

#endif
