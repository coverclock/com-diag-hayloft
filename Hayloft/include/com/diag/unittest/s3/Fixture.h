/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_S3_FIXTURE
#define _H_COM_DIAG_UNITTEST_S3_FIXTURE

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include "gtest/gtest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/hayloft/s3/Credentials.h"

namespace com {
namespace diag {
namespace unittest {
namespace s3 {

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;

/*******************************************************************************
 * ENVIRONMENT
 ******************************************************************************/

class Environment : public ::testing::Environment {

private:

	static ::testing::Environment * const environment;

	// During development, the unit tests read the Digital Aggregates access key
	// id and secret access key from files on disk. But you can also set the
	// appropriate environmental variables. The unit tests will use those values
	// and do the right thing to test the underlying code. Note that the HOME
	// environmental variable must be set to use the files.

	static const char * ACCESS_KEY_ID_PATH() { return "/projects/hayloft/aws-s3-access-key-id.txt"; }
	static const char * SECRET_ACCESS_KEY_PATH() { return "/projects/hayloft/aws-s3-secret-access-key.txt"; }

	const std::string HOME;
	const std::string ACCESS_KEY_ID_FILE;
	const std::string SECRET_ACCESS_KEY_FILE;

	static std::string access_key_id;
	static std::string secret_access_key;

public:

	static const char * getAccessKeyId() { return access_key_id.c_str(); }
	static const char * getSecretAccessKey() { return secret_access_key.c_str(); }

	Environment()
	: HOME((std::getenv("HOME") != 0) ? std::getenv("HOME") : "")
	, ACCESS_KEY_ID_FILE(HOME + ACCESS_KEY_ID_PATH())
	, SECRET_ACCESS_KEY_FILE(HOME + SECRET_ACCESS_KEY_PATH())
	{}

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
			}
		}
	}

	virtual void TearDown() {
	}

};

std::string Environment::access_key_id;
std::string Environment::secret_access_key;

::testing::Environment * const Environment::environment = ::testing::AddGlobalTestEnvironment(new Environment);

/*******************************************************************************
 * FIXTURE
 ******************************************************************************/

class Fixture : public ::testing::Test {

public:

	static ::com::diag::desperado::Print printf;
	static ::com::diag::desperado::Dump dump;

	static ::com::diag::desperado::FileOutput errput;
	static ::com::diag::desperado::LogOutput logput;

private:

	::com::diag::desperado::Output * output;
	Logger::Mask mask;

protected:

	virtual void SetUp() {
		output = &(Logger::instance().getOutput());
		mask = Logger::instance().getMask();
		Logger::instance()
			.setOutput(logput)
	    	.enable(Logger::FINEST)
	    	.enable(Logger::FINER)
	    	.enable(Logger::FINE)
	    	.enable(Logger::TRACE)
	    	.enable(Logger::DEBUG)
	    	.enable(Logger::INFORMATION)
	    	.enable(Logger::CONFIGURATION)
	    	.enable(Logger::NOTICE)
	    	.enable(Logger::WARNING)
	    	.enable(Logger::ERROR)
	    	.enable(Logger::SEVERE)
	    	.enable(Logger::CRITICAL)
	    	.enable(Logger::ALERT)
	    	.enable(Logger::FATAL)
	    	.enable(Logger::EMERGENCY)
	    	.enable(Logger::PRINT);
	}

	virtual void TearDown() {
		Logger::instance().setMask(mask);
		Logger::instance().setOutput(*output);
	}

};

::com::diag::desperado::FileOutput Fixture::errput(::stderr);
::com::diag::desperado::LogOutput Fixture::logput(Fixture::errput);
::com::diag::desperado::Print Fixture::printf(Fixture::errput);
::com::diag::desperado::Dump Fixture::dump(Fixture::errput);

}
}
}
}

#endif
