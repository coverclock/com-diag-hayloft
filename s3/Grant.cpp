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
, grants(new ::S3AclGrant [COUNT])
{
	initialize(0);
}

Grant::Grant(const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, keypointer(0)
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(0);
}

Grant::Grant(const Bucket & bucket, const Grant & grant)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, keypointer(0)
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(&grant);
}

Grant::Grant(const Object & object, const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(0);
}

Grant::Grant(const Object & object, const Bucket & bucket, const Grant & grant)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(&grant);
}

Grant::Grant(const Bucket & bucket, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, keypointer(0)
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(0);
}

Grant::Grant(const Bucket & bucket, const Grant & grant, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, keypointer(0)
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(&grant);
}

Grant::Grant(const Object & object, const Bucket & bucket, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(new ::S3AclGrant [COUNT])
{
	initialize(0);
}

Grant::Grant(const Object & object, const Bucket & bucket, const Grant & grant, const Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, key(object.getKey())
, keypointer(key.c_str())
, count(0)
, grants(new ::S3AclGrant [COUNT])
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
		if (this->count > COUNT) { this->count = COUNT; }
		std:memcpy(this->grants, that->grants, sizeof(that->grants[0]) * this->count);
	}
}

bool Grant::import(const char * xml) {
	bool result = false;
	char ownerid[OWNER_LEN];
	char ownerdisplayname[DISPLAY_LEN];
	// libs3 has a minor flaw (the only one I've found): S3_convert_acl()
	// doesn't declare the XML string const. But it can be const, because
	// it is merely passed to XML2's simplexml_add() which _does_ declare it
	// const. So I cheat here and hope it never comes back to bite me. I
	// control my risk somewhat by making a local copy of the XML string.
	std::string xmlstring = xml;
	::S3Status status = S3_convert_acl(
		const_cast<char *>(xmlstring.c_str()),
		ownerid,
		ownerdisplayname,
		&count,
		grants
	);
	if (status == ::S3StatusOK) {
		ownerid[sizeof(ownerid) - 1] = '\0';
		owner = ownerid;
		ownerdisplayname[sizeof(ownerdisplayname) - 1] = '\0';
		display = ownerdisplayname;
		result = true;
	}
	return result;
}

bool Grant::add(::S3GranteeType type, ::S3Permission permission, const char * addressOrOwnerId, const char * ownerDisplayName) {
	bool result = false;
	if (count < COUNT) {
		switch (type) {
		case ::S3GranteeTypeAmazonCustomerByEmail:
			if (addressOrOwnerId != 0) {
				grants->granteeType = type;
				grants->permission = permission;
				std::strncpy(grants->grantee.amazonCustomerByEmail.emailAddress, addressOrOwnerId, sizeof(grants->grantee.amazonCustomerByEmail.emailAddress));
				grants->grantee.amazonCustomerByEmail.emailAddress[sizeof(grants->grantee.amazonCustomerByEmail.emailAddress) - 1] = '\0';
				++count;
				result = true;
			}
			break;
		case ::S3GranteeTypeCanonicalUser:
			if ((addressOrOwnerId != 0) && (ownerDisplayName != 0)) {
				grants->granteeType = type;
				grants->permission = permission;
				std::strncpy(grants->grantee.canonicalUser.id, addressOrOwnerId, sizeof(grants->grantee.canonicalUser.id));
				this->grants->grantee.canonicalUser.id[sizeof(grants->grantee.canonicalUser.id) - 1] = '\0';
				std::strncpy(grants->grantee.canonicalUser.displayName, ownerDisplayName, sizeof(grants->grantee.canonicalUser.displayName));
				grants->grantee.canonicalUser.displayName[sizeof(grants->grantee.canonicalUser.displayName) - 1] = '\0';
				++count;
				result = true;
			}
			break;
		default:
			grants->granteeType = type;
			grants->permission = permission;
			++count;
			result = true;
			break;
		}
	}
	return result;
}

}
}
}
}
