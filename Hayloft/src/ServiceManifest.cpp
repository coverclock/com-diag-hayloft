/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include "com/diag/hayloft/ServiceManifest.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/set.h"
#include "com/diag/grandote/string.h"

namespace com {
namespace diag {
namespace hayloft {

ServiceManifest::Entry::Entry(const char * bucketname, const char * ownerId, const char * ownerDisplayName, Epochalseconds creationDateSeconds)
: canonical(bucketname)
, owner(::com::diag::grandote::set(ownerId))
, display(::com::diag::grandote::set(ownerDisplayName))
, created(creationDateSeconds)
{}

Status ServiceManifest::listServiceCallback(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds, void * callbackData) {
	ServiceManifest * that = static_cast<ServiceManifest*>(callbackData);
	Status status = that->entry(ownerId, ownerDisplayName, bucketName, creationDateSeconds);
	if ((bucketName != 0) && (bucketName != '\0')) {
		::com::diag::grandote::MaskableLogger::Level level = (status == ::S3StatusOK) ? ::com::diag::grandote::MaskableLogger::DEBUG : ::com::diag::grandote::MaskableLogger::NOTICE;
		Entry entry(bucketName, ownerId, ownerDisplayName, creationDateSeconds);
		::com::diag::grandote::MaskableLogger::instance().log(level, "ServiceManifest@%p: bucketName=\"%s\" ownerId=\"%s\" ownerDisplayName=\"%s\" creationDateSeconds=%lld\n", that, entry.getCanonical(), entry.getOwnerId(), entry.getOwnerDisplayName(), entry.getCreated());
		::com::diag::grandote::MaskableLogger::instance().log(level, "ServiceManifest@%p: status=%d=\"%s\"\n", that, status, tostring(status));
		if (status == ::S3StatusOK) {
			that->manifest.insert(Pair(bucketName, entry));
		}
	}
	return status;
}

ServiceManifest::ServiceManifest(const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol())
{
	initialize();
	execute();
}

ServiceManifest::ServiceManifest(const Plex & plex, const Context & context, const Session & session)
: Service(context.getId(), context.getSecret(), session.getEndpoint(), context.getProtocol(), plex)
{
	initialize();
}

ServiceManifest::~ServiceManifest() {
}

void ServiceManifest::initialize() {
	state(static_cast<Status>(IDLE));
	std::memset(&handler, 0, sizeof(handler));
	handler.responseHandler.propertiesCallback = Service::handler.propertiesCallback;
	handler.responseHandler.completeCallback = Service::handler.completeCallback;
	handler.listServiceCallback = &listServiceCallback;
}

void ServiceManifest::execute() {
	state(static_cast<Status>(BUSY));
	::com::diag::grandote::MaskableLogger::instance().debug("ServiceManifest@%p: begin\n", this);
	Service::execute();
	::S3_list_service(
		protocol,
		id.c_str(),
		secret.c_str(),
		endpoint.c_str(),
		handle,
		&handler,
		this
	);
}

bool ServiceManifest::start() {
	if (Service::start()) {
		execute();
		return true;
	} else {
		return false;
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

bool ServiceManifest::reset() {
	if (Service::reset()) {
		manifest.clear();
		return true;
	} else {
		return false;
	}
}

Status ServiceManifest::entry(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds) {
	::com::diag::grandote::MaskableLogger::instance().debug("ServiceManifest%p: entry\n", this);
	return ::S3StatusOK;
}

}
}
}
