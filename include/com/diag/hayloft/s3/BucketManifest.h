/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_BUCKETMANIFEST_
#define _H_COM_DIAG_HAYLOFT_S3_BUCKETMANIFEST_

/**
 * @file
 *
 * Copyright 2011-2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <map>
#include <list>
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Selection.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * BucketManifest is a Bucket Action which retrieves a manifest or table of
 * contents of the S3 objects in an S3 bucket.
 */
class BucketManifest : public Bucket {

public:

	/**
	 * Entry describes an entry in the Manifest map.
	 */
	class Entry {

	private:

		const std::string key;

		const Epochalseconds modified;

		const std::string etag;

		const Octets size;

		const std::string owner;

		const std::string display;

	public:

		/**
		 * Ctor.
		 *
		 * @param objectname is the name of the S3 object in the S3 bucket.
		 * @param lastModified is the date and time of last modification in
		 *        seconds since the UNIX Epoch.
		 * @param eTag is a C string that is the entity tag.
		 * @param objectsize is the object size in bytes (octets).
		 * @param ownerId is a C string that is the ID of the owner of the
		 *        S3 object.
		 * @param ownerDisplayName is a C string that is the display name of
		 *        the owner of the S3 object.
		 */
		explicit Entry(
			const char * objectname,
			Epochalseconds lastModified,
			const char * eTag,
			Octets objectsize,
			const char * ownerId,
			const char * ownerDisplayName
		);

		/**
		 * Get the key (object name).
		 *
		 * @return the key.
		 */
		const char * getKey() const { return key.c_str(); }

		/**
		 * Get the date and time of last modification in seconds since the UNIX
		 * Epoch.
		 *
		 * @return the date and time of last modification.
		 */
		Epochalseconds getModified() const { return modified; }

		/**
		 * Get the entity tag.
		 *
		 * @return the entity tag.
		 */
		const char * getETag() const { return etag.c_str(); }

		/**
		 * Get the size in bytes (octets).
		 *
		 * @return the size.
		 */
		Octets getSize() const { return size; }

		/**
		 * Get the owner ID.
		 *
		 * @return the owner ID.
		 */
		const char * getOwnerId() const { return owner.c_str(); }

		/**
		 * Get the owner display name.
		 *
		 * @return the owner display name.
		 */
		const char * getOwnerDisplayName() const { return display.c_str(); }

	};

	/**
	 * The Manifest map contains tuples of the Object name and its Entry.
	 */
	typedef std::map<std::string, Entry> Manifest;

	/**
	 * The Manifest pair is a tuple of the Object name and its Entry.
	 */
	typedef std::pair<std::string, Entry> Pair;

	/**
	 * The Common list contains strings of common Object name prefixes.
	 */
	typedef std::list<std::string> Common;

protected:

	static Status listBucketCallback(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes, void * callbackData);

	static void responseCompleteCallback(Status status, const ::S3ErrorDetails * errorDetails, void * callbackData);

	std::string prefix;

	std::string marker;

	std::string nextmarker;

	std::string delimiter;

	int maximum;

	bool truncated;

	Manifest manifest;

	Common common;

	::S3ListBucketHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name. A copy
	 *        is made of this C string.
	 * @param selection specifies a Selection criteria as to what object keys
	 *        are to be returned. See Selection.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 *        This reference is only used during construction.
	 */
	explicit BucketManifest(
		const char * bucketname,
		const Selection & selection = Selection(),
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucketname is the non-canonical (application) bucket name. A copy
	 *        is made of this C string.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param selection specifies a Selection criteria as to what object keys
	 *        are to be returned. See Selection.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Bucket.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Bucket.
	 */
	explicit BucketManifest(
		const char * bucketname,
		const Plex & plex,
		const Selection & selection = Selection(),
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 * @param selection specifies a Selection criteria as to what object keys
	 *        are to be returned. See Selection.
	 */
	explicit BucketManifest(
		const Bucket & bucket,
		const Selection & selection = Selection()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param bucket refers to a Bucket Action from which this Bucket Action
	 *        is configured. This reference is only used during construction.
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param selection specifies a Selection criteria as to what object keys
	 *        are to be returned. See Selection.
	 */
	explicit BucketManifest(
		const Bucket & bucket,
		const Plex & plex,
		const Selection & selection = Selection()
	);

	/**
	 * Dtor. If the Action is in the BUSY state this forces the execution of
	 * all Actions on the same Plex and blocks until they all complete.
	 */
	virtual ~BucketManifest();

	/**
	 * Start the Action if it is not busy or forced.
	 *
	 * @param force if true cause the start to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool start(bool force = false);

	/**
	 * Get a constant reference to the Manifest map.
	 *
	 * @return a constant reference to the Manifest map.
	 */
	const Manifest & getManifest() const { return manifest; }

	/**
	 * Get a reference to the Common list.
	 *
	 * @return a reference to the Common list.
	 */
	const Common & getCommon() const { return common; }

	/**
	 * Return a constant pointer to an Entry associated with the Object name if
	 * it is in the Manifest map, null otherwise.
	 *
	 * @param name is an Object name.
	 * @return a constant pointer to an Entry or null.
	 */
	virtual const Entry * find(const char * name) const;

	/**
	 * Reset the action if it is not busy or forced. This includes clearing
	 * the Manifest map and the Common list.
	 *
	 * @param force if true cause the start to be performed even if the Action
	 *              is busy. This option is used by the management system.
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset(bool force = false);

protected:

	/**
	 * This method is called when a bucket entry is returned by S3.
	 * Hayloft guarantees that this method does nothing except return
	 * ::S3StatusOK to indicate success. It can be safely overridden by the
	 * application for its own purposes. The overriding method does not need to
	 * call this method.
	 *
	 * @param isTruncated is non-zero if S3 is only returning a partial list of
	 *        the bucket contents.
	 * @param nextMarker is the C string that if non-null told S3 after which
	 *        object name to start returning a list of bucket contents. See
	 *        Selection.
	 * @param contentsCount is the number of positions in the contents array.
	 * @param contents is an array of libS3 ::S3ListBucketContent structures.
	 * @param commonPrefixesCount is the number of positions in the
	 *        commonPrefixes array. See Selection.
	 * @param commonPrefixes is an array of pointers to C strings containing
	 *        common object name prefixes. See Selection.
	 */
	virtual Status entry(int isTruncated, const char * nextMarker, int contentsCount, const S3ListBucketContent * contents, int commonPrefixesCount, const char ** commonPrefixes);

private:

	void initialize();

	void execute();

};

}
}
}
}

#endif
