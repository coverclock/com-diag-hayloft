/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_PROPERTIES_
#define _H_COM_DIAG_HAYLOFT_S3_PROPERTIES_

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
#include "com/diag/hayloft/set.h"
#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/s3/Access.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Properties {

public:

	typedef std::map<std::string, std::string> Metadata;

	typedef std::pair<std::string, std::string> Pair;

protected:

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Epochalseconds expiration;

	::S3CannedAcl access;

	Metadata metadata;

public:

	explicit Properties(
		const char * contentType = 0,
		const char * md5 = 0,
		const char * cacheControl = 0,
		const char * contentDispositionFilename = 0,
		const char * contentEncoding = 0,
		const Epochalseconds expires = -1,
		const Access & cannedAcl = Access()
	);

	virtual ~Properties() {}

	const char * getType() const { return type.c_str(); }

	const char * getChecksum() const { return checksum.c_str(); }

	const char * getControl() const { return control.c_str(); }

	const char * getFilename() const { return filename.c_str(); }

	const char * getEncoding() const { return encoding.c_str(); }

	Epochalseconds getExpires() const { return expiration; }

	::S3CannedAcl getAccess() const { return access; }

	Properties & setType(const char * contentType = 0) { type = set(contentType, 0, ""); return *this; }

	Properties & setChecksum(const char * md5 = 0) { checksum = set(md5, 0, ""); return *this; }

	Properties & setControl(const char * cacheControl = 0) { control = set(cacheControl, 0, ""); return *this; }

	Properties & setFilename(const char * contentDispositionFilename = 0) { filename = set(contentDispositionFilename, 0, ""); return *this; }

	Properties & setEncoding(const char * contentEncoding = 0) { encoding = set(contentEncoding, 0, ""); return *this; }

	Properties & setExpires(const Epochalseconds expires = -1) { expiration = expires; return *this; }

	Properties & setAccess(const Access & cannedAcl = Access()) { access = cannedAcl.getAccess(); return *this; }

	const Metadata & getMetadata() const { return metadata; }

	/**
	 * Insert a (keyword,value) tuple into the metadata. Avoid
	 * special characters in the key C string. Also, S3 will convert all the
	 * upper case alphas in the key C string to lower case when it stores
	 * them.
	 *
	 * @param key is a C string that is the keyword.
	 * @param value is a C string that is the value.
	 * @return a reference to this object.
	 */
	Properties & insert(const char * key, const char * value) { metadata.insert(Pair(key, value)); return *this; }

	Properties & erase(const char * key) { metadata.erase(key); return *this; }

	const char * find(const char * key) const;

};

}
}
}
}

#endif
