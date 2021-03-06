/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_OBJECTGET_
#define _H_COM_DIAG_HAYLOFT_OBJECTGET_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <string>
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/Conditions.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace grandote {
class Output;
}
}
}

namespace com {
namespace diag {
namespace hayloft {

class Bucket;

/**
 * ObjectPut is an Object Action which gets an S3 object from an S3 bucket. By
 * default, a Grandote Output functor is used as the application data sink.
 * Other data sinks can be used by either implementing the Grandote Output
 * functor interface, or by overriding the appropriate methods in this object.
 * When Grandote Output functors are used, the functors can be either UNTAKEN
 * (passed by reference) or TAKEN (passed by pointer). In the latter case, the
 * TAKEN functor will be deleted when this Action completes. (Some Grandote
 * Input functors rely on destruction to close the underlying data sink.)
 */
class ObjectGet : public Object {

public:

	typedef ::com::diag::grandote::Output Output;

protected:

	static Status getObjectDataCallback(int bufferSize, const char * buffer, void * callbackData);

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	Epochalseconds since;

	Epochalseconds notsince;

	std::string match;

	std::string notmatch;

	Output * output;

	Output * taken;

	Octets offset;

	Octets total;

	Octets produced;

	::S3GetConditions conditions;

	::S3GetObjectHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
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
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        this Action completes.
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
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
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
		const Plex & plex,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        this Action completes.
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
		const Plex & plex,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const Object & object,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const Object & object,
		Output * sinkp, /* TAKEN */
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
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
		const Object & object,
		const Plex & plex,
		Output & sink,
		Octets objectoffset = 0,
		Octets objectsize = 0,
		const Conditions & conds = Conditions()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param conds refers to the Conditions associated with this get.
	 *        This reference is only used during construction.
	 */
	explicit ObjectGet(
		const Object & object,
		const Plex & plex,
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
	 * Gets the number of octets produced so far to the output functor.
	 * Used to determine of the output functor is soiled.
	 *
	 * @return the number of octets produced.
	 */
	Octets getProduced() const { return produced; }

	/**
	 * Start this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool start();

	/**
	 * Reset this Action if none of the data sink has been produced. Success
	 * means this Action can be retried without a new Output functor.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset();

	/**
	 * Reset this Action by resetting the data sink to a new Output functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Output & sink, Octets objectoffset = 0, Octets objectsize = 0);

	/**
	 * Reset this Action by resetting the data sink to a new Output functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Output * sinkp /* TAKEN */, Octets objectoffset = 0, Octets objectsize = 0);

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
	 * @return the number of bytes actually written, zero for End Of File, or
	 *         negative with an error number set for an error.
	 */
	virtual int get(int bufferSize, const void * buffer);

	/**
	 * Handle the close processing of the underlying data sink. A derived class
	 * can override this function to perform its own close processing, but
	 * should call this method.
	 */
	virtual void finalize();

private:

	void initialize();

	void execute();

};

}
}
}

#endif
