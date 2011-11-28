/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTCOPY_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTCOPY_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Properties.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class ObjectCopy : public Object {

public:

	typedef int64_t Epochalseconds;

	static const size_t LENGTH = 256;

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string tocanonical;

	std::string toname;

	std::string tokey;

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Properties::Epochalseconds expires;

	::S3CannedAcl access;

	Properties::Metadata metadata;

	std::string etag;

	Epochalseconds modified;

	char tag[LENGTH];

	::S3PutProperties properties;

	::S3ResponseHandler handler;

public:

	explicit ObjectCopy(
		const char * fromkeyname,
		const Bucket & frombucket,
		const char * tokeyname,
		const Bucket & tobucket,
		const Properties & props = Properties()
	);

	explicit ObjectCopy(
		const char * fromkeyname,
		const Bucket & frombucket,
		const char * tokeyname,
		const Bucket & tobucket,
		Multiplex & multiplex,
		const Properties & props = Properties()
	);

	virtual ~ObjectCopy();

	virtual void start();

	const char * getCanonicalTo() const { return tocanonical.c_str(); }

	const char * getNameTo() const { return toname.c_str(); }

	const char * getKeyTo() const { return tokey.c_str(); }

	Epochalseconds getModified() const { return modified; }

	const char * getTag() const { return etag.c_str(); }

private:

	void initialize(const Properties::Metadata & settings);

	void execute();

};

}
}
}
}

#endif
