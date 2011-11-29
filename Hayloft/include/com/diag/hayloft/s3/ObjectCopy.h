/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_OBJECTCOPY_
#define _H_COM_DIAG_HAYLOFT_S3_OBJECTCOPY_

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
namespace hayloft {
namespace s3 {

class Bucket;

/**
 * ObjectCopy is an Object Action which makes a copy of an S3 object in an S3
 * bucket. The copy can be placed in a different S3 bucket, in the same S3
 * bucket under a different key or obejct name, or even to the same S3 object
 * which has the effect of altering its meta-data.
 */
class ObjectCopy : public Object {

public:

	/**
	 * S3 object modification time encoded as seconds past the UNIX Epoch.
	 */
	typedef int64_t Epochalseconds;

	/**
	 * The maximum length of an eTag C string returned by this Action.
	 */
	static const size_t LENGTH = 256;

private:

	static void responseCompleteCallback(::S3Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

protected:

	std::string tocanonical;

	std::string toname;

	std::string tokey;

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Properties::Epochalseconds expires;

	::S3CannedAcl access;

	Properties::Metadata metadata;

	std::string etag;

	Epochalseconds modified;

	char tag[LENGTH];

	::S3PutProperties properties;

	::S3ResponseHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param fromkeyname is the name of this Object from which the copy is
	 *        made.
	 * @param frombucket refers to the Bucket associated with this object from
	 *        which the copy is made. This reference is only used during
	 *        construction.
	 * @param tokeyname is the name of that Object to which the copy is made.
	 * @param tobucket refers to the Bucket to which the copy is made. This
	 *        reference is only used during construction.
	 * @param props refers to the Properties to be associated with the object
	 *        to which the copy is being made. This reference is only used
	 *        during construction.
	 */
	explicit ObjectCopy(
		const char * fromkeyname,
		const Bucket & frombucket,
		const char * tokeyname,
		const Bucket & tobucket,
		const Properties & props = Properties()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param fromkeyname is the name of this Object from which the copy is
	 *        made.
	 * @param frombucket refers to the Bucket associated with this object from
	 *        which the copy is made. This reference is only used during
	 *        construction.
	 * @param tokeyname is the name of that Object to which the copy is made.
	 * @param tobucket refers to the Bucket to which the copy is made. This
	 *        reference is only used during construction.
	 * @param multiplex refers to the Multiplex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param props refers to the Properties to be associated with the object
	 *        to which the copy is being made. This reference is only used
	 *        during construction.
	 */
	explicit ObjectCopy(
		const char * fromkeyname,
		const Bucket & frombucket,
		const char * tokeyname,
		const Bucket & tobucket,
		Multiplex & multiplex,
		const Properties & props = Properties()
	);

	/**
	 * Dtor.
	 */
	virtual ~ObjectCopy();

	/**
	 * Start the Action if it is IDLE, or re-start it if it is neither IDLE nor
	 * BUSY.
	 */
	virtual void start();

	/**
	 * Return the canonical name of the bucket to which the copy is made.
	 * @return the canonical name of the bucket to which the copy is made.
	 */
	const char * getCanonicalTo() const { return tocanonical.c_str(); }

	/**
	 * Return the non-canonical (application) anme of the bucket to which the
	 * copy is made.
	 * @return the non-canonical (application) anme of the bucket to which the
	 * copy is made.
	 */
	const char * getNameTo() const { return toname.c_str(); }

	/**
	 * Return the name of the object to which the copy is made.
	 * @return name of the object to which the copy is made.
	 */
	const char * getKeyTo() const { return tokey.c_str(); }

	/**
	 * Return the modification time provided by S3 once this Action completes.
	 * @return the modification time.
	 */
	Epochalseconds getModified() const { return modified; }

	/**
	 * Return the eTag provided by S3 once this Action completes.
	 * @return the eTag.
	 */
	const char * getTag() const { return etag.c_str(); }

private:

	void initialize(const Properties::Metadata & settings);

	void execute();

};

}
}
}
}

#endif
