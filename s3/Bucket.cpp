/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 * http://libs3.ischo.com.s3.amazonaws.com/index.html<BR>
 */

#include <cstdlib>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/Logger.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Bucket::Bucket(const char * bucketName)
: name(bucketName)
{

}

Bucket::~Bucket() {
}

bool Bucket::valid(Session & session, const UniversalResourceIdentifierStyle & style) {
	std::string bucketName = name;
	bucketName += session.getBucketSuffix();
	::S3Status status = S3_validate_bucket_name(bucketName.c_str(), style.getUniversalResourceIdentifierStyle());
	if (status != ::S3StatusOK) {
		Logger::instance().debug("Bucket@%p: S3_validate_bucket_name failed! name=\"%s\"[%zu] status=%d=\"%s\"\n", this, bucketName.c_str(), bucketName.length(), status, ::S3_get_status_name(status));
	}
	return (status == ::S3StatusOK);
}

void Bucket::test(Session & session, const Credentials & credentials, const Protocol & protocol, const UniversalResourceIdentifierStyle & style) {
	std::string bucketName = name;
	bucketName += session.getBucketSuffix();
}

void Bucket::create(Session & session, const Credentials & credentials, const LocationConstraint & constraint, const Protocol & protocol, const UniversalResourceIdentifierStyle & style, const CannedAccessControlList & acl) {

}

void Bucket::remove(Session & session, const Credentials & credentials, const LocationConstraint & constraint, const Protocol & protocol,const UniversalResourceIdentifierStyle & style) {

}

}
}
}
}
