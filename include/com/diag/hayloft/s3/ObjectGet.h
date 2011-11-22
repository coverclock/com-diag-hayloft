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

public:

	typedef ::com::diag::desperado::Output Output;

private:

	static ::S3Status getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData);

protected:

	Conditions::Epochalseconds since;

	Conditions::Epochalseconds notsince;

	std::string match;

	std::string notmatch;

	Output * output;

	Output * taken;

	Octets offset;

	Octets size;

	::S3GetConditions conditions;

	::S3GetObjectHandler handler;

public:

	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	virtual ~ObjectGet();

	void start() { if (state() != BUSY) { execute(); } }

	void reset(Output & sink, Octets objectoffset = 0, Octets objectsize = 0) { if ((state() != BUSY)) { output = &sink; taken = 0; offset = objectoffset; size = objectsize; }}

	void reset(Output * sinkp /* TAKEN */, Octets objectoffset = 0, Octets objectsize = 0) { if ((state() != BUSY)) { output = sinkp; taken = sinkp; offset = objectoffset; size = objectsize; } }

	bool isGotten() const { return (state() == ::S3StatusOK); }

protected:

	virtual ::S3Status get(int bufferSize, const char * buffer);

	virtual void complete(::S3Status status, const ::S3ErrorDetails * errorDetails);

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
