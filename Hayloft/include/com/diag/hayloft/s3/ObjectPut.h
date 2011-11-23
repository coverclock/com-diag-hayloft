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

public:

	typedef ::com::diag::desperado::Input Input;

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

	Input * input;

	Input * taken;

	Octets size;

	Properties::Metadata metadata;

	::S3PutProperties properties;

	::S3PutObjectHandler handler;

public:

	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Input * sourcep, /* TAKEN */
		Octets objectsize,
		const Properties & props = Properties()
	);

	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Input * sourcep, /* TAKEN */
		Octets objectsize,
		const Properties & props = Properties()
	);

	virtual ~ObjectPut();

	virtual void start();

	virtual void reset(Input & source, Octets objectsize);

	virtual void reset(Input * sourcep /* TAKEN */, Octets objectsize);

protected:

	virtual int put(int bufferSize, char * buffer);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize(const Properties::Metadata & settings);

	void execute();

	void finalize();

};

}
}
}
}

#endif
