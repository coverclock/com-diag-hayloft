/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTPUT
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTPUT

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
namespace desperado {
class Input;
}
}
}

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class ObjectPut : public Object {

private:

	static int putObjectDataCallback(int bufferSize, char * buffer, void * callbackData);

protected:

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Properties::Metadata metadata;

	Octets size;

	::S3BucketContext context;

	::S3PutProperties properties;

	::S3PutObjectHandler handler;

	::com::diag::desperado::Input * inputp;

	::com::diag::desperado::Input & input;

public:

	explicit ObjectPut(
		const Bucket & bucket,
		const char * keyname,
		Octets objectsize,
		::com::diag::desperado::Input & source,
		 const Properties & props = Properties()
	);

	explicit ObjectPut(
		const Bucket & bucket,
		const char * keyname,
		Octets objectsize,
		::com::diag::desperado::Input * sourcep, /* TAKEN */
		 const Properties & props = Properties()
	);

	virtual ~ObjectPut();

protected:

	virtual int put(int bufferSize, char * buffer);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize(const Properties & props);

};

}
}
}
}

#endif
