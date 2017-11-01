/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_PROPERTIES_
#define _H_COM_DIAG_HAYLOFT_PROPERTIES_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <map>
#include <string>
#include "com/diag/grandote/set.h"
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/Access.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Properties defines the metadata that can be applied to an S3 object during
 * Actions like ObjectPut and ObjectCopy. (S3 does not appear to propagate
 * the metadata of the source S3 object to the sink S3 object in an ObjectCopy
 * Action.)
 */
class Properties {

public:

	/**
	 * ObjectPut and ObjectCopy apply metadata in the form of (keyword,value)
	 * tuples to S3 objects. This is a map containing those tuples.
	 */
	typedef std::map<std::string, std::string> Metadata;

	/**
	 * ObjectPut and ObjectCopy apply metadata in the form of (keyword,value)
	 * tuples to S3 objects. This is a tuple.
	 */
	typedef std::pair<std::string, std::string> Pair;

protected:

	std::string type;

	std::string checksum;

	std::string control;

	std::string filename;

	std::string encoding;

	Epochalseconds expiration;

	::S3CannedAcl access;

	Metadata metadata;

public:

	/**
	 * Ctor.
	 *
	 * @param contentType is the content type, which defaults to
	 *        "binary/octet-stream".
	 * @param md5 is an MD5 signature that is typically used as a hash table
	 *        key or as a checksum.
	 * @param cacheControl provides a cache control HTTP header.
	 * @param contentDispositionFilename changes the filename the S3 object is
	 *        downloaded to when accessed by a web browser.
	 * @param contentEncoding identifies the content encoding for a web browser
	 *        so that it can (for example) decompress a compressed file.
	 * @param expires is a time represented as seconds since the UNIX Epoch that
	 *        identifies an expiration date to a web browser.
	 * @param cannedAcl specifies the predefined ACL to be applied to the S3
	 *        object. This reference is only used during construction.
	 */
	explicit Properties(
		const char * contentType = 0,
		const char * md5 = 0,
		const char * cacheControl = 0,
		const char * contentDispositionFilename = 0,
		const char * contentEncoding = 0,
		const Epochalseconds expires = -1,
		const Access & cannedAcl = Access()
	);

	/**
	 * Dtor.
	 */
	virtual ~Properties() {}

	/**
	 * Get the content type.
	 *
	 * @return the content type.
	 */
	const char * getType() const { return type.c_str(); }

	/**
	 * Get the MD5 checksum.
	 *
	 * @return the MD5 checksum.
	 */
	const char * getChecksum() const { return checksum.c_str(); }

	/**
	 * Get the cache control.
	 *
	 * @return the cache control.
	 */
	const char * getControl() const { return control.c_str(); }

	/**
	 * Get the content disposition filename.
	 *
	 * @return the content disposition filename.
	 */
	const char * getFilename() const { return filename.c_str(); }

	/**
	 * Get the content encoding.
	 *
	 * @return the content encoding.
	 */
	const char * getEncoding() const { return encoding.c_str(); }

	/**
	 * Get the expiration date and time.
	 *
	 * @return the expiration date and time.
	 */
	Epochalseconds getExpires() const { return expiration; }

	/**
	 * Get the canned access control list.
	 *
	 * @return the canned access control list.
	 */
	::S3CannedAcl getAccess() const { return access; }

	/**
	 * Set the content type.
	 *
	 * @param contentType is the content type, which defaults to
	 *        "binary/octet-stream".
	 * @return a reference to this object.
	 */
	Properties & setType(const char * contentType = 0) { type = ::com::diag::grandote::set(contentType); return *this; }

	/**
	 * Set the MD5 checksum.
	 *
	 * @param md5 is an MD5 signature that is typically used as a hash table
	 *        key or as a checksum.
	 * @return a reference to this object.
	 */
	Properties & setChecksum(const char * md5 = 0) { checksum = ::com::diag::grandote::set(md5); return *this; }

	/**
	 * Set the cache control.
	 *
	 * @param cacheControl provides a cache control HTTP header.
	 * @return a reference to this object.
	 */
	Properties & setControl(const char * cacheControl = 0) { control = ::com::diag::grandote::set(cacheControl); return *this; }

	/**
	 * Set the content disposition filename.
	 *
	 * @param contentDispositionFilename changes the filename the S3 object is
	 *        downloaded to when accessed by a web browser.
	 * @return a reference to this object.
	 */
	Properties & setFilename(const char * contentDispositionFilename = 0) { filename = ::com::diag::grandote::set(contentDispositionFilename); return *this; }

	/**
	 * Set the content encoding.
	 *
	 * @param contentEncoding identifies the content encoding for a web browser
	 *        so that it can (for example) decompress a compressed file.
	 * @return a reference to this object.
	 */
	Properties & setEncoding(const char * contentEncoding = 0) { encoding = ::com::diag::grandote::set(contentEncoding); return *this; }

	/**
	 * Set the expiration date and time.
	 *
	 * @param expires is a time represented as seconds since the UNIX Epoch that
	 *        identifies an expiration date to a web browser.
	 * @return a reference to this object.
	 */
	Properties & setExpires(const Epochalseconds expires = -1) { expiration = expires; return *this; }

	/**
	 * Set the canned access control list.
	 *
	 * @param cannedAcl specifies the predefined ACL to be applied to the S3
	 *        object. This reference is only used during construction.
	 * @return a reference to this object.
	 */
	Properties & setAccess(const Access & cannedAcl = Access()) { access = cannedAcl.getAccess(); return *this; }

	/**
	 * Get a constant reference to the metadata map.
	 *
	 * @return a constant reference to the metadata map.
	 */
	const Metadata & getMetadata() const { return metadata; }

	/**
	 * Insert a (keyword,value) tuple into the metadata. Avoid
	 * special characters in the key C string. Also, S3 will convert all the
	 * upper case alphas in the key C string to lower case when it stores
	 * them.
	 *
	 * @param key is a C string that is the keyword.
	 * @param value is a C string that is the value.
	 * @return a reference to this object.
	 */
	Properties & insert(const char * key, const char * value) { metadata.insert(Pair(key, value)); return *this; }

	/**
	 * Erase a (keyword,value) tuple into the metadata. Avoid
	 * special characters in the key C string. Also, S3 will convert all the
	 * upper case alphas in the key C string to lower case when it stores
	 * them.
	 *
	 * @param key is a C string that is the keyword.
	 * @return a reference to this object.
	 */
	Properties & erase(const char * key) { metadata.erase(key); return *this; }

	/**
	 * If there is a metadata entry with the given key, return its value,
	 * otherwise return null.
	 *
	 * @param key is the metadata keyword.
	 * @return a metadata value for the key or null.
	 */
	const char * find(const char * key) const;

private:

	/*
	 * The copy constructor and assignment operator aren't poisoned here,
	 * because the synthesized versions will work and won't be functionally
	 * different from explicitly defined ones. But copying an object of this
	 * type is likely to be expensive.
	 */

};

}
}
}

#endif
