/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_PROPERTIES
#define _H_COM_DIAG_HAYLOFT_S3_PROPERTIES

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <map>
#include <string>
#include "com/diag/hayloft/s3/Access.h"
#include "com/diag/hayloft/Logger.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Properties {

public:

	typedef std::map<std::string, std::string> Metadata;

	typedef std::pair<std::string, std::string> Pair;

private:

	::S3PutProperties properties;

	std::string contentType;

	std::string md5;

	std::string cacheControl;

	std::string contentDispositionFilename;

	std::string contentEncoding;

	Metadata metadata;

public:

	explicit Properties(
		const char * ct = 0,
		const char * m5 = 0,
		const char * cc = 0,
		const char * cd = 0,
		const char * ce = 0,
		const int64_t ex = -1,
		const Access & ac = Access(),
		const Metadata & md = Metadata()
	);

	virtual ~Properties();

	const ::S3PutProperties * getProperties() const { return &properties; }

};

extern void show(const ::S3PutProperties * properties, Logger::Level level = Logger::DEBUG);

}
}
}
}

#endif
