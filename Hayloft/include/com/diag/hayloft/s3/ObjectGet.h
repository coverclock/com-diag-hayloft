/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTGET_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTGET_

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
#include "com/diag/hayloft/s3/Conditions.h"
#include "com/diag/desperado/target.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace desperado {
class Output;
}
}
}

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Bucket;

class ObjectGet : public Object {

private:

	static ::S3Status getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData);

protected:

	Conditions::Epochalseconds since;

	Conditions::Epochalseconds notsince;

	std::string match;

	std::string notmatch;

	::com::diag::desperado::Output * outputp;

	::com::diag::desperado::Output & output;

	Octets offset;

	Octets size;

	ssize_t produced;

	::S3GetConditions conditions;

	::S3GetObjectHandler handler;

public:

	explicit ObjectGet(
		const Bucket & bucket,
		const char * keyname,
		::com::diag::desperado::Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	explicit ObjectGet(
		const Bucket & bucket,
		const char * keyname,
		::com::diag::desperado::Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	virtual ~ObjectGet();

	bool isGotten() const { return (status == ::S3StatusOK); }

	bool isInaccessible() const { return (status == ::S3StatusHttpErrorForbidden); }

	bool isNonexistent() const { return (status == ::S3StatusHttpErrorNotFound); }

protected:

	virtual ::S3Status get(int bufferSize, const char * buffer);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

	void begin();

};

}
}
}
}

#endif
