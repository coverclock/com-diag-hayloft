/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTPUT_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTPUT_

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
#include "com/diag/desperado/target.h"
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

	Properties::Epochalseconds expires;

	::S3CannedAcl access;

	::com::diag::desperado::Input * inputp;

	::com::diag::desperado::Input & input;

	Octets size;

	ssize_t consumed;

	Properties::Metadata metadata;

	::S3PutProperties properties;

	::S3PutObjectHandler handler;

public:

	explicit ObjectPut(
		const Bucket & bucket,
		const char * keyname,
		::com::diag::desperado::Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	explicit ObjectPut(
		const Bucket & bucket,
		const char * keyname,
		::com::diag::desperado::Input * sourcep, /* TAKEN */
		Octets objectsize,
		const Properties & props = Properties()
	);

	virtual ~ObjectPut();

	bool isPut() const { return (status == ::S3StatusOK); }

	bool isInaccessible() const { return (status == ::S3StatusHttpErrorForbidden); }

protected:

	virtual int put(int bufferSize, char * buffer);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize(const Properties::Metadata & settings);

	void begin();

};

}
}
}
}

#endif
