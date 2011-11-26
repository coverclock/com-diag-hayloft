/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/BucketList.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

BucketList::Entry::Entry(const char * ownerId, const char * ownerDisplayName, Epochalseconds creationDateSeconds)
: id((ownerId != 0) ? ownerId : "")
, display((ownerDisplayName != 0) ? ownerDisplayName : "")
, created(creationDateSeconds)
{}

::S3Status BucketList::listServiceCallback(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds, void * callbackData) {
	BucketList * that = static_cast<BucketList*>(callbackData);
	::S3Status status = that->entry(ownerId, ownerDisplayName, bucketName, creationDateSeconds);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	const char * effective = (bucketName != 0) ? bucketName : "";
	Entry entry(ownerId, ownerDisplayName, creationDateSeconds);
	Logger::instance().log(level, "BucketList@%p: bucketName=\"%s\" ownerId=\"%s\" ownerDisplayName=\"%s\" creationDateSeconds=%lld\n", that, effective, entry.id.c_str(), entry.display.c_str(), entry.created);
	Logger::instance().log(level, "BucketList@%p: status=%d=\"%s\"\n", that, status, ::S3_get_status_name(status));
	if (status == ::S3StatusOK) {
		that->list.insert(Pair(effective, entry));
	}
	return status;
}

BucketList::BucketList(const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol())
{
	initialize();
	execute();
}

BucketList::BucketList(Multiplex & multiplex, const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol(), multiplex)
{
	initialize();
}

BucketList::~BucketList() {
}

void BucketList::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Service::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Service::handler.completeCallback;
	handler.listServiceCallback = &listServiceCallback;
}

void BucketList::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("BucketList@%p: begin\n", this);
	::S3_list_service(
		protocol,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		requests,
		&handler,
		this
	);
}

const BucketList::Entry * BucketList::find(const char * name) const {
	const Entry * entry = 0;
	List::const_iterator here = list.find(name);
	if (here != list.end()) {
		entry = &(here->second);
	}
	return entry;
}

void BucketList::start() {
	if ((state() == BUSY) && (requests != 0)) {
		execute();
	}
}

::S3Status BucketList::entry(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds) {
	return ::S3StatusOK;
}

void BucketList::complete(::S3Status status, const ::S3ErrorDetails * errorDetails) {
	Service::complete(status, errorDetails);
	Logger::instance().debug("BucketList@%p: end\n", this);
}

}
}
}
}
