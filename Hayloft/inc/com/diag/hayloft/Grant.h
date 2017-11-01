/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_GRANT_
#define _H_COM_DIAG_HAYLOFT_GRANT_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <list>
#include <string>
#include "com/diag/hayloft/Container.h"
#include "com/diag/grandote/target.h"

namespace com {
namespace diag {
namespace hayloft {

class Bucket;
class Object;

/**
 * Grant represents an S3 access control list (ACL) for an S3 bucket or object.
 * Grants can be retrieved from an existing bucket or object, imported
 * from an Extensible Markup Language (XML) file in a pre-defined format, or
 * created from scratch.
 *
 * Remarkably, if you set an ACL entry using a user's electronic mail address,
 * S3 appears to convert this to a canonical user entry, which is how it
 * appears when you get it back.
 */
class Grant : public Container {

public:

	/**
	 * Entry describes an entry in the access control list.
	 */
	class Entry {

	private:

		::S3GranteeType type;

		::S3Permission permission;

		std::string owner;

		std::string display;

	public:

		/**
		 * Ctor.
		 *
		 * @param granteeType is the libs3 grantee type.
		 * @param permissionType is the libs3 permission.
		 * @param ownerIdOrEmailAddress is null or a C string that is an owner
		 *        ID or an electronic mail address, depending on the grantee
		 *        type.
		 * @param ownerDisplayName is the null or a C string that is an owner
		 *        display name, depending on the grantee type.
		 */
		explicit Entry(
			::S3GranteeType granteeType,
			::S3Permission permissionType,
			const char * ownerIdOrEmailAddress,
			const char * ownerDisplayName
		);

		/**
		 * Get the grantee type.
		 *
		 * @return the grantee type.
		 */
		::S3GranteeType getGranteeType() const { return type; }

		/**
		 * Get the permission.
		 *
		 * @return the permission.
		 */
		::S3Permission getPermission() const { return permission; }

		/**
		 * Get the C string that is the owner ID, electronic mail address, or
		 * empty, dpending on the grantee type.
		 *
		 * @return the owner ID or email address.
		 */
		const char * getOwnerIdOrEmailAddress() const { return ((type == ::S3GranteeTypeCanonicalUser) || (type == ::S3GranteeTypeAmazonCustomerByEmail)) ? owner.c_str() : ""; }

		/**
		 * Get the C string that is the owner display name or empty, depending
		 * on the grantee type.
		 *
		 * @return the owner display name.
		 */
		const char * getOwnerDisplayName() const { return (type == ::S3GranteeTypeCanonicalUser) ?  display.c_str() : ""; }

	};

	/**
	 * The list contains each individual entry in the access control list.
	 */
	typedef std::list<Entry> List;

private:

	static int dontcare;

protected:

	std::string key;

	const char * keypointer;

	std::string owner;

	std::string display;

	List list;

public:

	/**
	 * Ctor. Use this for don't care.
	 */
	explicit Grant();

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Grant & grant
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Bucket & bucket,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Object & object,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to the Bucket associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Bucket & bucket,
		const Plex & plex,
		const Grant & grant = Grant()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param object refers to the Object associated with this object. This
	 *        reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param grant refers to another grant whose access control list is to be
	 *        associated with this object. This reference is only used during
	 *        construction.
	 */
	explicit Grant(
		const Object & object,
		const Plex & plex,
		const Grant & grant = Grant()
	);

	/**
	 * Dtor.
	 */
	virtual ~Grant();

	/**
	 * Get the key pointer associated with this Object. The key pointer is
	 * NULL if this is a grant for a bucket, non-null if for an object.
	 *
	 * @return the key pointer.
	 */
	const char * getKeyPointer() const { return keypointer; }

	/**
	 * Get the key (object name) associated with this Object.
	 *
	 * @return the key (object name).
	 */
	const char * getKey() const { return key.c_str(); }

	/**
	 * Get the content type provided by S3 once this Action completes.
	 * @return the content type.
	 */
	const char * getOwnerId() const { return owner.c_str(); }

	/**
	 * Get the eTag provided by S3 once this Action completes.
	 * @return the eTag.
	 */
	const char * getOwnerDisplayName() const { return display.c_str(); }

	/**
	 * Set the owner ID for this Grant. S3 requires this and that it be valid.
	 *
	 * @param ownerId is a C string that is the owner ID.
	 * @return a reference to this object.
	 */
	Grant & setOwnerId(const char * ownerId) { owner = ownerId; return *this; }

	/**
	 * Set the owner display name for this Grant. S3 requires this and that it
	 * be valid.
	 */
	Grant & setOwnerDisplayName(const char * ownerDisplayName) { display = ownerDisplayName; return *this; }

	/**
	 * Get a const reference to the access control list.
	 *
	 * @return a const reference ot the access control list.
	 */
	const List & getAccessControlList() const { return list; }

	/**
	 * Import (add) a new access control list entry.
	 *
	 * @param granteeType is the libs3 grantee type.
	 * @param permissionType is the libs3 permission.
	 * @param ownerIdOrEmailAddress is null or a C string that is an owner
	 *        ID or an electronic mail address, depending on the grantee
	 *        type.
	 * @param ownerDisplayName is the null or a C string that is an owner
	 *        display name, depending on the grantee type.
	 * @return the number of grant entries imported (nominally one).
	 */
	int import(::S3GranteeType granteeType, ::S3Permission permissionType, const char * ownerIdOrEmailAddress = 0, const char * ownerDisplayName = 0);

	/**
	 * Import (add) new access control list entries from a C string containing
	 * XML.
	 *
	 * @param xml points to the XML C string.
	 * @return the number of grant entries imported (zero likely implies a
	 *         syntax error in the XML file).
	 */
	int import(const char * xml);

	/**
	 * Import (add) new access control list entries from a libs3 ACL grant
	 * structure array.
	 *
	 * @param count is the number of entries in the array.
	 * @param grants points to the array.
	 * @return the number of grant entries imported (anything less than count
	 *         likely implies an error).
	 */
	int import(int count, ::S3AclGrant * grants);

	/**
	 * Import (add) new access control list entries from an existing Grant
	 * object.
	 *
	 * @param grant refers to an existing Grant object.
	 * @return the number of grant entries imported.
	 */
	int import(const Grant & grant);

	/**
	 * Generate an array of libs3 ACL grant structures and return the number
	 * of structures in the array.
	 *
	 * Note that "export" remains a reserved word in C++ (having to do with
	 * templates) even though it's functionality has been removed from the ISO
	 * C++0x version standard.
	 *
	 * @param count refers to a variable into which the number of structures in
	 *        the array is returned.
	 * @return a pointer to a dynamically acquired array of grant structures
	 *         for which the caller is responsible for deleting.
	 */
	::S3AclGrant * generate(int & count = dontcare) const;

private:

	void initialize(const Grant * that);

};

}
}
}

#endif
