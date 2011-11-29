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

/**
 * ObjectPut is an Object Action which gets an S3 object from an S3 bucket. By
 * default, a Desperado Output functor is used as the application data sink.
 * Other data sinks can be used by either implementing the Desperado Output
 * functor interface, or by overriding the appropriate methods in this object.
 * When Desperado Output functors are used, the functors can be either UNTAKEN
 * (passed by reference) or TAKEN (passed by pointer). In the latter case, the
 * TAKEN functor will be deleted when the Action completes. (Some Desperado
 * Input functors rely on destruction to close the underlying data sink.)
 */
class ObjectGet : public Object {

public:

	typedef ::com::diag::desperado::Output Output;

protected:

	static ::S3Status getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

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

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectGet();

	/**
	 * Start the Action if it is IDLE, or re-start it if it is neither IDLE nor
	 * BUSY.
	 */
	virtual void start();

	/**
	 * If the Action is not BUSY, reset the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 */
	virtual void reset(Output & sink, Octets objectoffset = 0, Octets objectsize = 0);

	/**
	 * If the Action is not BUSY, reset the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 */
	virtual void reset(Output * sinkp = 0 /* TAKEN */, Octets objectoffset = 0, Octets objectsize = 0);

protected:

	/**
	 * Handle the output processing of the underlying data sink as a result of
	 * getting an S3 object (I know, it seems backwards). This method uses the
	 * specified Output functor. It can be overridden in a derived class to
	 * implement other data sinks.
	 *
	 * @param bufferSize is the amount of data to output. The output is expected
	 *        to be atomic (that is, no partial writes).
	 * @param buffer points to the data to write.
	 * @return ::S3StatusOK to continue or any other libs3 ::S3Status value to
	 *         immediately abort the operation.
	 */
	virtual ::S3Status get(int bufferSize, const void * buffer);

private:

	void initialize();

	void execute();

	/**
	 * Handle the close processing of the underlying data sink. A derived class
	 * can override this function to perform its own close processing, but
	 * should call this method.
	 */
	virtual void finalize();

};

}
}
}
}

#endif
