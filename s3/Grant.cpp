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
#include "com/diag/hayloft/set.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

int Grant::dontcare = 0;

Grant::Entry::Entry(::S3GranteeType granteeType, ::S3Permission permissionType, const char * ownerIdOrEmailAddress, const char * ownerDisplayName)
: type(granteeType)
, permission(permissionType)
, owner(((granteeType == ::S3GranteeTypeAmazonCustomerByEmail) || (granteeType == ::S3GranteeTypeCanonicalUser)) ? set(ownerIdOrEmailAddress) : "")
, display((granteeType == ::S3GranteeTypeCanonicalUser) ? set(ownerDisplayName) : "")
{}

Grant::Grant()
: Container("", "", "", "", Protocol::DEFAULT, Style::DEFAULT)
, keypointer(0)
{
	initialize(0);
}

Grant::Grant(const Grant & grant)
: Container("", "", "", "", Protocol::DEFAULT, Style::DEFAULT)
, keypointer(0)
{
	initialize(&grant);
}

Grant::Grant(const Bucket & bucket, const Grant & grant)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle())
, keypointer(0)
{
	initialize(&grant);
}

Grant::Grant(const Object & object, const Grant & grant)
: Container(object.getId(), object.getSecret(), object.getEndpoint(), object.getCanonical(), object.getProtocol(), object.getStyle())
, key(object.getKey())
, keypointer(key.c_str())
{
	initialize(&grant);
}

Grant::Grant(const Bucket & bucket, const Multiplex & multiplex, const Grant & grant)
: Container(bucket.getId(), bucket.getSecret(), bucket.getEndpoint(), bucket.getCanonical(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, keypointer(0)
{
	initialize(&grant);
}

Grant::Grant(const Object & object, const Multiplex & multiplex, const Grant & grant)
: Container(object.getId(), object.getSecret(), object.getEndpoint(), object.getCanonical(), object.getProtocol(), object.getStyle(), multiplex)
, key(object.getKey())
, keypointer(key.c_str())
{
	initialize(&grant);
}

Grant::~Grant() {
}

void Grant::initialize(const Grant * that) {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(Logger::DEBUG)) {
		if (keypointer != 0) { logger.debug("Grant@%p: key=\"%s\"\n", this, key.c_str()); }
	}
	if (that != 0) {
		this->owner = that->owner;
		this->display = that->display;
		import(*that);
	}
}

int Grant::import(::S3GranteeType granteeType, ::S3Permission permissionType, const char * ownerIdOrEmailAddress, const char * ownerDisplayName) {
	Entry entry(granteeType, permissionType, ownerIdOrEmailAddress, ownerDisplayName);
	list.push_back(entry);
	return 1;
}

int Grant::import(int count, ::S3AclGrant * grants) {
	for (int ii = 0; ii < count; ++ii) {
		switch (grants[ii].granteeType) {
			case ::S3GranteeTypeAmazonCustomerByEmail: {
				grants[ii].grantee.amazonCustomerByEmail.emailAddress[sizeof(grants[ii].grantee.amazonCustomerByEmail.emailAddress) - 1] = '\0';
				Entry entry(grants[ii].granteeType, grants[ii].permission, grants[ii].grantee.amazonCustomerByEmail.emailAddress, "");
				list.push_back(entry);
			} break;
			case ::S3GranteeTypeCanonicalUser: {
				grants[ii].grantee.canonicalUser.id[sizeof(grants[ii].grantee.canonicalUser.id) - 1] = '\0';
				grants[ii].grantee.canonicalUser.displayName[sizeof(grants[ii].grantee.canonicalUser.displayName) - 1] = '\0';
				Entry entry(grants[ii].granteeType, grants[ii].permission, grants[ii].grantee.canonicalUser.id, grants[ii].grantee.canonicalUser.displayName);
				list.push_back(entry);
			} break;
			default: {
				Entry entry(grants[ii].granteeType, grants[ii].permission, "", "");
				list.push_back(entry);
			} break;
		}
	}
	return count;
}

int Grant::import(const char * xml) {
	int result = 0;
	char ownerid[OWNER_LEN];
	char ownerdisplayname[DISPLAY_LEN];
	int count;
	::S3AclGrant * grants = new ::S3AclGrant [COUNT];
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
		result = import(count, grants);
	}
	delete [] grants;
	return result;
}

int Grant::import(const Grant & grant) {
	int result = 0;
	if (&grant != this) {
		List::const_iterator here = grant.list.begin();
		List::const_iterator there = grant.list.end();
		while (here != there) {
			Entry entry(here->getGranteeType(), here->getPermission(), here->getOwnerIdOrEmailAddress(), here->getOwnerDisplayName());
			list.push_back(entry);
			++here;
			++result;
		}
	}
	return result;
}

::S3AclGrant * Grant::generate(int & count /* static int dontcare */) const {
	::S3AclGrant * grants = new ::S3AclGrant [list.size()];
	int index = 0;
	List::const_iterator here = list.begin();
	List::const_iterator there = list.end();
	while (here != there) {
		grants[index].granteeType = here->getGranteeType();
		grants[index].permission = here->getPermission();
		switch (here->getGranteeType()) {
		case ::S3GranteeTypeAmazonCustomerByEmail:
			std::strncpy(grants[index].grantee.amazonCustomerByEmail.emailAddress, here->getOwnerIdOrEmailAddress(), sizeof(grants[index].grantee.amazonCustomerByEmail.emailAddress));
			grants[index].grantee.amazonCustomerByEmail.emailAddress[sizeof(grants[index].grantee.amazonCustomerByEmail.emailAddress) - 1] = '\0';
			break;
		case ::S3GranteeTypeCanonicalUser:
			std::strncpy(grants[index].grantee.canonicalUser.id, here->getOwnerIdOrEmailAddress(), sizeof(grants[index].grantee.canonicalUser.id));
			grants[index].grantee.canonicalUser.id[sizeof(grants[index].grantee.canonicalUser.id) - 1] = '\0';
			std::strncpy(grants[index].grantee.canonicalUser.displayName, here->getOwnerDisplayName(), sizeof(grants[index].grantee.canonicalUser.displayName));
			grants[index].grantee.canonicalUser.displayName[sizeof(grants[index].grantee.canonicalUser.displayName) - 1] = '\0';
			break;
		default:
			break;
		}
		++here;
		++index;
	}
	count = index;
	return grants;
}

}
}
}
}
