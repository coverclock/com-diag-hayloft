/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/tostring.h"
#include "com/diag/hayloft/s3/Object.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const char * tostring(::S3Status status) {
	const char * result;
	if      (status == static_cast<S3Status>(Action::BUSY))	{ result = "BUSY"; }
	else if (status == static_cast<S3Status>(Action::IDLE))	{ result = "IDLE"; }
	else													{ result = ::S3_get_status_name(status); }
	return result;
}

const char * tostring(::S3Protocol protocol) {
	const char * result = "Invalid";
	switch (protocol) {
	case ::S3ProtocolHTTPS:	result = "https";	break;
	case ::S3ProtocolHTTP:	result = "http";	break;
	// No default generates warning for missing case.
	}
	return result;
}

const char * tostring(::S3UriStyle style) {
	const char * result = "Invalid";
	switch (style) {
	case ::S3UriStyleVirtualHost:	result = "Virtual Host";	break;
	case ::S3UriStylePath:			result = "Path";			break;
	// No default generates warning for missing case.
	}
	return result;
}

const char * tostring(::S3CannedAcl access) {
	const char * result = "Invalid";
	switch (access) {
	case ::S3CannedAclPrivate:				result = "Private";				break;
	case ::S3CannedAclPublicRead:			result = "Public Read";			break;
	case ::S3CannedAclPublicReadWrite:		result = "Public Read Write";	break;
	case ::S3CannedAclAuthenticatedRead:	result = "Authenticated Read";	break;
	// No default generates warning for missing case.
	}
	return result;
}

const char * tostring(const Container & container, std::string & uri) {
	uri = tostring(container.getProtocol());
	uri += "://";
	switch (container.getStyle()) {
	case ::S3UriStyleVirtualHost: /* PROTOCOL://CANONICAL.ENDPOINT/KEY */
		uri += container.getCanonical();
		uri += '.';
		uri += container.getEndpoint();
		break;
	case ::S3UriStylePath: /* PROTOCOL://ENDPOINT/CANONICAL/KEY */
		uri += container.getEndpoint();
		uri += '/';
		uri += container.getCanonical();
		break;
	}
	return uri.c_str();
}

const char * tostring(const Object & object, std::string & uri) {
	tostring(static_cast<Container>(object), uri);
	uri += '/';
	uri += object.getKey();
	return uri.c_str();
}

}
}
}
}
