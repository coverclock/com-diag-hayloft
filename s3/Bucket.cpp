/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
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

bool Bucket::valid(Context & context) {
	std::string bucketName = name;
	bucketName += context.getSession().getBucketSuffix();
	::S3Status status = S3_validate_bucket_name(bucketName.c_str(), context.getUniversalResourceIdentifierStyle().getStyle());
	if (status != ::S3StatusOK) {
		Logger::instance().debug("Bucket@%p: S3_validate_bucket_name failed! name=\"%s\"[%zu] status=%d=\"%s\"\n", this, bucketName.c_str(), bucketName.length(), status, ::S3_get_status_name(status));
	}
	return (status == ::S3StatusOK);
}

void Bucket::test(Context & context) {
	std::string bucketName = name;
	bucketName += context.getSession().getBucketSuffix();
}

void Bucket::create(Context & context) {

}

void Bucket::remove(Context & context) {

}

}
}
}
}
