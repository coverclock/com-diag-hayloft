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
#include "com/diag/hayloft/s3/types.h"
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

/**
 * ObjectPut is an Object Action which puts an S3 object into an S3 bucket,
 * overwriting any existing S3 object with the same key. By default, a Desperado
 * Input functor is used as the application data source. Other data sources can
 * be used by either implementing the Desperado Input functor interface, or by
 * overriding the appropriate methods in this object. When Desperado Input
 * functors are used, the functors can be either UNTAKEN (passed by reference)
 * or TAKEN (passed by pointer). In the latter case, the TAKEN functor will be
 * deleted when the Action completes. (Some Desperado Output functors rely on
 * destruction to close the underlying data source.)
 */
class ObjectPut : public Object {

public:

	typedef ::com::diag::desperado::Input Input;

private:

	static int putObjectDataCallback(int bufferSize, char * buffer, void * callbackData);

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Epochalseconds expires;

	::S3CannedAcl access;

	Input * input;

	Input * taken;

	Octets size;

	Properties::Metadata metadata;

	::S3PutProperties properties;

	::S3PutObjectHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Input * sourcep = 0, /* TAKEN */
		Octets objectsize = 0,
		const Properties & props = Properties()
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
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
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
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		Multiplex & multiplex,
		Input * sourcep = 0, /* TAKEN */
		Octets objectsize = 0,
		const Properties & props = Properties()
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectPut();

	/**
	 * Start the Action if it is IDLE, or re-start it if it is neither IDLE nor
	 * BUSY.
	 */
	virtual void start();

	/**
	 * If the Action is not BUSY, reset the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 */
	virtual void reset(Input & source, Octets objectsize);

	/**
	 * If the Action is not BUSY, reset the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        the Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 */
	virtual void reset(Input * sourcep = 0 /* TAKEN */, Octets objectsize = 0);

protected:

	/**
	 * Handle the input processing of the underlying data source as a result of
	 * putting an S3 object (I know, it seems backwards). This method uses the
	 * specified Input functor. It can be overridden in a derived class to
	 * implement other data sources.
	 *
	 * @param bufferSize is the amount of data to input. Partial reads are okay.
	 * @param buffer points to the buffer into which data is read.
	 * @return the actual number of bytes actually read or zero for End Of File.
	 */
	virtual int put(int bufferSize, void * buffer);

private:

	void initialize(const Properties::Metadata & settings);

	void execute();

	/**
	 * Handle the close processing of the underlying data source. A derived
	 * class can override this function to perform its own close processing, but
	 * should call this method.
	 */
	void finalize();

};

}
}
}
}

#endif
