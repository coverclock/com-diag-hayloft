/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Grant.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

int Grant::dontcare = 0;

Grant::Grant()
: Container("", "", "", "", Protocol::DEFAULT, Style::DEFAULT)
, keypointer(0)
, count(0)
, grants(0)
{
	initialize(0);
}

Grant::Grant(const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, keypointer(0)
, count(0)
, grants(0)
{
	initialize(0);
}

Grant::Grant(const Object & object, const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(0)
{
	initialize(0);
}

Grant::Grant(const Grant & grant)
: Container(grant.getId(), grant.getSecret(), grant.getEndpoint(), grant.getCanonical(), grant.getProtocol(), grant.getStyle())
, key(grant.getKey())
, keypointer(key.c_str())
, count(0)
, grants(0)
{
	initialize(&grant);
}

Grant::Grant(const Bucket & bucket, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, keypointer(0)
, count(0)
, grants(0)
{
	initialize(0);
}

Grant::Grant(const Object & object, const Bucket & bucket, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(0)
{
	initialize(0);
}

Grant::Grant(const Grant & grant, const Multiplex & multiplex)
: Container(grant.getId(), grant.getSecret(), grant.getEndpoint(), grant.getCanonical(), grant.getProtocol(), grant.getStyle(), multiplex)
, key(grant.getKey())
, keypointer(key.c_str())
, count(0)
, grants(0)
{
	initialize(&grant);
}

Grant::~Grant() {
	delete [] grants;
}

void Grant::initialize(const Grant * that) {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		if (keypointer != 0) { logger.debug("Grant@%p: key=\"%s\"\n", this, key.c_str()); }
	}
	if ((that != 0) && (that != this)) {
		this->key = that->key;
		this->keypointer = that->keypointer;
		this->owner = that->owner;
		this->display = that->display;
		this->count = that->count;
		this->grants = new ::S3AclGrant [COUNT];
		std:memcpy(this->grants, that->grants, sizeof(that->grants[0]) * this->count);
	}
}

}
}
}
}
