/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_OBJECTPUT_
#define _H_COM_DIAG_HAYLOFT_OBJECTPUT_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/Properties.h"
#include "com/diag/hayloft/S3.h"
#include "com/diag/grandote/target.h"

namespace com {
namespace diag {
namespace grandote {
class Input;
}
}
}

namespace com {
namespace diag {
namespace hayloft {

class Bucket;

/**
 * ObjectPut is an Object Action which puts an S3 object into an S3 bucket,
 * overwriting any existing S3 object with the same key. By default, a Desperado
 * Input functor is used as the application data source. Other data sources can
 * be used by either implementing the Desperado Input functor interface, or by
 * overriding the appropriate methods in this object. When Desperado Input
 * functors are used, the functors can be either UNTAKEN (passed by reference)
 * or TAKEN (passed by pointer). In the latter case, the TAKEN functor will be
 * deleted when this Action completes. (Some Desperado Output functors rely on
 * destruction to close the underlying data source.)
 */
class ObjectPut : public Object {

public:

	typedef ::com::diag::grandote::Input Input;

protected:

	static int putObjectDataCallback(int bufferSize, char * buffer, void * callbackData);

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Epochalseconds expires;

	::S3CannedAcl access;

	Input * input;

	Input * taken;

	Octets total;

	Octets consumed;

	::S3PutProperties properties;

	::S3PutObjectHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
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
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
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
	 * @param keyname is the name of this Object. A copy is made of this C
	 *        string.
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		const Plex & plex,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
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
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const char * keyname,
		const Bucket & bucket,
		const Plex & plex,
		Input * sourcep = 0, /* TAKEN */
		Octets objectsize = 0,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const Object & object,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const Object & object,
		Input * sourcep = 0, /* TAKEN */
		Octets objectsize = 0,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const Object & object,
		const Plex & plex,
		Input & source,
		Octets objectsize,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to a Object Action from which this Object Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads (or at least
	 *        not when libs3 was written).
	 * @param props refers to the Properties to be associated with the object.
	 *        This reference is only used during construction.
	 */
	explicit ObjectPut(
		const Object & object,
		const Plex & plex,
		Input * sourcep = 0, /* TAKEN */
		Octets objectsize = 0,
		const Properties & props = Properties()
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectPut();

	/**
	 * Gets the number of octets consumed so far from the input functor.
	 * Used to determine if the input functor is soiled.
	 *
	 * @return the number of octets consumed.
	 */
	Octets getConsumed() const { return consumed; }

	/**
	 * Start this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool start();

	/**
	 * Reset this Action if none of the data source has been consumed. Success
	 * means this Action can be retried without a new Input functor.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset();

	/**
	 * Reset this Action by resetting the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param source refers to an Input functor.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Input & source, Octets objectsize);

	/**
	 * Reset this Action by resetting the data source to a new Input functor.
	 * This can be used in retry and error recover strategies.
	 *
	 * @param sourcep points to an Input functor which is TAKEN and deleted when
	 *        this Action completes.
	 * @param objectsize is the size of the data source in eight-bit bytes. S3
	 *        requires this and does not support partial uploads.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(Input * sourcep /* TAKEN */, Octets objectsize);

protected:

	/**
	 * Handle the input processing of the underlying data source as a result of
	 * putting an S3 object (I know, it seems backwards). This method uses the
	 * specified Input functor. It can be overridden in a derived class to
	 * implement other data sources.
	 *
	 * @param bufferSize is the amount of data to input. Partial reads are okay.
	 * @param buffer points to the buffer into which data is read.
	 * @return the number of bytes actually read, zero for End Of File, or
	 *         negative with an error number set for an error.
	 */
	virtual int put(int bufferSize, void * buffer);

	/**
	 * Handle the close processing of the underlying data source. A derived
	 * class can override this function to perform its own close processing, but
	 * should call this method.
	 */
	virtual void finalize();

private:

	void initialize(const Properties::Metadata & settings);

	void execute();

};

}
}
}

#endif
