/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_LOG_
#define _H_COM_DIAG_HAYLOFT_S3_LOG_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Grant.h"
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Plex;

/**
 *
 */
class Log : public Grant {

protected:

	std::string target;

	std::string prefix;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 * @param log refers to the bucket into which the logs are to be written.
	 *        This reference is only used during construction.
	 * @param keyprefix is an optional C string that is a key (object name)
	 *        prefix used for any logs generated.
	 * @param grant refers to a Grant whose access control list will be applied
	 *        to any of the logs generated. This reference is only used during
	 *        construction.
	 */
	explicit Log(
		const Bucket & bucket,
		const Bucket & log = Bucket(),
		const char * keyprefix = 0,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers about to the bucket whose access is to be logged.
	 *        This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param log refers to the bucket into which the logs are to be written.
	 *        This reference is only used during construction.
	 * @param keyprefix is an optional C string that is a key (object name)
	 *        prefix used for any logs generated.
	 * @param grant refers to a Grant whose access control list will be applied
	 *        to any of the logs generated. This reference is only used during
	 *        construction.
	 */
	explicit Log(
		const Bucket & bucket,
		const Plex & plex,
		const Bucket & log = Bucket(),
		const char * keyprefix = 0,
		const Grant & grant = Grant()
	);

	/**
	 * Dtor.
	 */
	virtual ~Log();

	const char * getTarget() const { return target.c_str(); }

	const char * getPrefix() const { return prefix.c_str(); }

private:

	void initialize();

};

}
}
}
}

#endif