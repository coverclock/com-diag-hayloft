/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/ServiceManifest.h"
#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

ServiceManifest::Entry::Entry(const char * ownerId, const char * ownerDisplayName, Epochalseconds creationDateSeconds)
: id((ownerId != 0) ? ownerId : "")
, display((ownerDisplayName != 0) ? ownerDisplayName : "")
, created(creationDateSeconds)
{}

::S3Status ServiceManifest::listServiceCallback(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds, void * callbackData) {
	ServiceManifest * that = static_cast<ServiceManifest*>(callbackData);
	::S3Status status = that->entry(ownerId, ownerDisplayName, bucketName, creationDateSeconds);
	Logger::Level level = (status == ::S3StatusOK) ? Logger::DEBUG : Logger::NOTICE;
	const char * effective = (bucketName != 0) ? bucketName : "";
	Entry entry(ownerId, ownerDisplayName, creationDateSeconds);
	Logger::instance().log(level, "ServiceManifest@%p: bucketName=\"%s\" ownerId=\"%s\" ownerDisplayName=\"%s\" creationDateSeconds=%lld\n", that, effective, entry.id.c_str(), entry.display.c_str(), entry.created);
	Logger::instance().log(level, "ServiceManifest@%p: status=%d=\"%s\"\n", that, status, tostring(status));
	if (status == ::S3StatusOK) {
		that->manifest.insert(Pair(effective, entry));
	}
	return status;
}

ServiceManifest::ServiceManifest(const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol())
{
	initialize();
	execute();
}

ServiceManifest::ServiceManifest(Multiplex & multiplex, const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol(), multiplex)
{
	initialize();
}

ServiceManifest::~ServiceManifest() {
}

void ServiceManifest::initialize() {
	status = static_cast<S3Status>(IDLE); // Why not static_cast<::S3Status>(IDLE)?
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Service::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Service::handler.completeCallback;
	handler.listServiceCallback = &listServiceCallback;
}

void ServiceManifest::execute() {
	status = static_cast<S3Status>(BUSY); // Why not static_cast<::S3Status>(BUSY)?
	Logger::instance().debug("ServiceManifest@%p: begin\n", this);
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

void ServiceManifest::start() {
	if ((state() == BUSY) && (requests != 0)) {
		execute();
	}
}

const ServiceManifest::Entry * ServiceManifest::find(const char * name) const {
	const Entry * entry = 0;
	Manifest::const_iterator here = manifest.find(name);
	if (here != manifest.end()) {
		entry = &(here->second);
	}
	return entry;
}

void ServiceManifest::reset() {
	if ((state() != BUSY)) {
		manifest.clear();
	}
}

::S3Status ServiceManifest::entry(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds) {
	Logger::instance().debug("ServiceManifest%p: entry\n", this);
	return ::S3StatusOK;
}

}
}
}
}
