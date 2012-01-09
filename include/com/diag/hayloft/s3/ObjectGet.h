/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTGET_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTGET_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Conditions.h"
#include "com/diag/hayloft/s3/S3.h"

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
	 *        the Action completes.
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
	 *        the Action completes.
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
	 * Start the Action if it is not busy or forced.
	 *
	 * @param force if true cause the start to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool start(bool force = false);

	/**
	 * Reset the action if it is not busy or forced and none of the data sink
	 * has been produced. Success means the Action can be retried without a new
	 * Output functor.
	 *
	 * @param force if true cause the reset to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(bool force = false);

	/**
	 * If the Action is not busy, reset the data sink to a new Output functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sink refers to an Output functor.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param force if true cause the reset to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Output & sink, Octets objectoffset = 0, Octets objectsize = 0, bool force = false);

	/**
	 * If the Action is not busy, reset the data sink to a new Output functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sinkp points to an Output functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectoffset is the offset in eight-bit bytes from the beginning
	 *        of the object at which the get begins.
	 * @param objectsize is the size of the data source in eight-bit bytes to
	 *        be retrieved or zero to retrieve the entire object.
	 * @param force if true cause the reset to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Output * sinkp /* TAKEN */, Octets objectoffset = 0, Octets objectsize = 0, bool force = false);

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
}

#endif
