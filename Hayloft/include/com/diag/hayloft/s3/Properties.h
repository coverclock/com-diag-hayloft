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
#include "com/diag/desperado/target.h"
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

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	int64_t expiration;

	::S3CannedAcl access;

	Metadata metadata;

public:

	explicit Properties(
		const char * contentType = 0,
		const char * md5 = 0,
		const char * cacheControl = 0,
		const char * contentDispositionFilename = 0,
		const char * contentEncoding = 0,
		const int64_t expires = -1,
		const Access & cannedAcl = Access()
	);

	virtual ~Properties() {}

	const char * getType() const { return type.c_str(); }

	const char * getChecksum() const { return checksum.c_str(); }

	const char * getControl() const { return control.c_str(); }

	const char * getFilename() const { return filename.c_str(); }

	const char * getEncoding() const { return encoding.c_str(); }

	int64_t getExpires() const { return expiration; }

	::S3CannedAcl getAccess() const { return access; }

	const Metadata & getMetadata() const { return metadata; }

	Properties & insert(const char * key, const char * value) { metadata.insert(Pair(key, value)); return *this; }

	Properties & erase(const char * key) { metadata.erase(key); return *this; }

	const char * find(const char * key) const;

};

}
}
}
}

#endif
