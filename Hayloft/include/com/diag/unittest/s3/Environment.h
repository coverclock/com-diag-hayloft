/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_ENVIRONMENT_
#define _H_COM_DIAG_UNITTEST_S3_ENVIRONMENT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 *
 * During unit testing, if the following parameters
 * 	Access Key ID (a twenty-character "login"),
 * 	Secret Access Key (a forty-character "password"), and
 * 	User Agent (a uniquefier),
 * are not found in the environment, they are read from files on disk and
 * exported into the environment. But you can also set the appropriate
 * environmental variables, the names of which can be found in the Credentials
 * and Session header file. The unit tests will use those values and do the
 * right thing to test the underlying code. Note that the HOME environmental
 * variable must be set to use the files.
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/string.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Bucket.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

class Environment : public ::testing::Environment {

private:

	static ::testing::Environment * const environment;

	static const char * ACCESS_KEY_ID_PATH() { return "/projects/hayloft/aws-s3-access-key-id.txt"; }
	static const char * SECRET_ACCESS_KEY_PATH() { return "/projects/hayloft/aws-s3-secret-access-key.txt"; }
	static const char * USER_AGENT_PATH() { return "/projects/hayloft/aws-s3-user-agent.txt"; }

	static const std::string HOME;
	static const std::string ACCESS_KEY_ID_FILE;
	static const std::string SECRET_ACCESS_KEY_FILE;
	static const std::string USER_AGENT_FILE;

	std::string access_key_id;
	std::string secret_access_key;
	std::string user_agent;

public:

	Environment() {}

	virtual ~Environment() {}

	virtual void SetUp() {
		const char * env;

		if ((env = std::getenv(Credentials::ACCESS_KEY_ID_ENV())) != 0) {
			access_key_id = env;
		} else {
			char val[Credentials::ACCESS_KEY_ID_LEN + sizeof("\n")];
			::com::diag::desperado::PathInput input(ACCESS_KEY_ID_FILE.c_str(), "r");
			size_t len = input(val, sizeof(val));
			if (len > 0) {
				if (val[len - 2] == '\n') { val[len - 2] = '\0'; }
				access_key_id = val;
				::setenv(Credentials::ACCESS_KEY_ID_ENV(), access_key_id.c_str(), !0);
			} else {
				// Do nothing.
			}
		}

		if ((env = std::getenv(Credentials::SECRET_ACCESS_KEY_ENV())) != 0) {
			secret_access_key = env;
		} else {
			char val[Credentials::SECRET_ACCESS_KEY_LEN + sizeof("\n")];
			::com::diag::desperado::PathInput input(SECRET_ACCESS_KEY_FILE.c_str(), "r");
			size_t len = input(val, sizeof(val));
			if (len > 0) {
				if (val[len - 2] == '\n') { val[len - 2] = '\0'; }
				secret_access_key = val;
				::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), secret_access_key.c_str(), !0);
			} else {
				// Do nothing.
			}
		}

		if ((env = std::getenv(Session::USER_AGENT_ENV())) != 0) {
			user_agent = env;
		} else {
			char val[Bucket::LENGTH + sizeof("\n")];
			::com::diag::desperado::PathInput input(USER_AGENT_FILE.c_str(), "r");
			size_t len = input(val, sizeof(val));
			if (len > 0) {
				if (val[len - 2] == '\n') { val[len - 2] = '\0'; }
				user_agent = val;
				::setenv(Session::USER_AGENT_ENV(), user_agent.c_str(), !0);
			} else {
				// Do nothing.
			}
		}

	}

	virtual void TearDown() {
	}

};


const std::string Environment::HOME((std::getenv("HOME") != 0) ? std::getenv("HOME") : "");
const std::string Environment::ACCESS_KEY_ID_FILE(Environment::HOME + Environment::ACCESS_KEY_ID_PATH());
const std::string Environment::SECRET_ACCESS_KEY_FILE(Environment::HOME + Environment::SECRET_ACCESS_KEY_PATH());
const std::string Environment::USER_AGENT_FILE(Environment::HOME + Environment::USER_AGENT_PATH());

::testing::Environment * const Environment::environment = ::testing::AddGlobalTestEnvironment(new Environment);

}
}
}
}

#endif
