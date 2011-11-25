/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETLIST_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETLIST_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <map>
#include <string>
#include "com/diag/hayloft/s3/Service.h"
#include "com/diag/hayloft/s3/Context.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/desperado/target.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class BucketList : public Service {

public:

	typedef int64_t Epochalseconds;

	struct Entry {
		std::string id;
		std::string display;
		Epochalseconds created;
	};

	typedef std::map<std::string, Entry> List;

	typedef std::pair<std::string, Entry> Pair;

private:

	static ::S3Status listServiceCallback(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds, void * callbackData);

protected:

	List list;

	::S3ListServiceHandler handler;

public:

	explicit BucketList(
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	explicit BucketList(
		Multiplex & multiplex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	virtual ~BucketList();

	virtual void start();

	const List & getList() const { return list; }

	const Entry * find(const char * name) const;

protected:

	virtual ::S3Status service(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds);

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
