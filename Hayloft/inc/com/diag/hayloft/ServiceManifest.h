/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_SERVICEMANIFEST_
#define _H_COM_DIAG_HAYLOFT_SERVICEMANIFEST_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <map>
#include <string>
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/Service.h"
#include "com/diag/hayloft/Context.h"
#include "com/diag/hayloft/Session.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * ServiceManifest is a Service which retrieves a manifest or table of
 * contents of the S3 buckets associated with a set of Credentials.
 */
class ServiceManifest : public Service {

public:

	/**
	 * Entry describes an entry in the Manifest map.
	 */
	class Entry {

	private:

		const std::string canonical;

		const std::string owner;

		const std::string display;

		const Epochalseconds created;

	public:

		/**
		 * Ctor.
		 *
		 * @param bucketname is the canonical bucket name.
		 * @param ownerId is the ID of the owner of the bucket.
		 * @param ownerDisplayName is the display name of the owner of the
		 *        bucket.
		 * @param creationDateSeconds is the date and time of creation in
		 *        seconds since the UNIX epoch.
		 */
		explicit Entry(
			const char * bucketname,
			const char * ownerId,
			const char * ownerDisplayName,
			Epochalseconds creationDateSeconds
		);

		/**
		 * Get the canonical bucket name.
		 *
		 * @return the canonical bucket name.
		 */
		const char * getCanonical() const { return canonical.c_str(); }

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

		/**
		 * Get the date and time of creation in seconds since the UNIX epoch.
		 *
		 * @return the date and time of creation.
		 */
		Epochalseconds getCreated() const { return created; }

	};

	/**
	 * The Manifest map contains tuples of the canonical Bucket name and its
	 * Entry.
	 */
	typedef std::map<std::string, Entry> Manifest;

	/**
	 * The Manifest pair is a tuple of the canonical Bucket name and its Entry.
	 */
	typedef std::pair<std::string, Entry> Pair;

protected:

	static Status listServiceCallback(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds, void * callbackData);

	Manifest manifest;

	::S3ListServiceHandler handler;

public:

	/**
	 * Ctor. Use this for the synchronous interface.
	 *
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Service.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Service.
	 *        This reference is only used during construction.
	 */
	explicit ServiceManifest(
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Ctor. Use this for the asynchronous interface.
	 *
	 * @param plex refers to the Plex responsible for executing this
	 *        Action asynchronously. This reference is only used during
	 *        construction.
	 * @param context refers to a Context object which provides the Credentials,
	 *        Region, Protocol, Style, and Access associated with this Service.
	 *        This reference is only used during construction.
	 * @param session refers to a Session object associated with this Service.
	 */
	explicit ServiceManifest(
		const Plex & plex,
		const Context & context = Context(),
		const Session & session = Session::instance()
	);

	/**
	 * Dtor. If this Action is in the BUSY state this forces the execution of
	 * all Actions on the same Plex and blocks until they all complete.
	 */
	virtual ~ServiceManifest();

	/**
	 * Start this Action.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool start();

	/**
	 * Get a constant reference to the Manifest map.
	 *
	 * @return a constant reference to the Manifest map.
	 */
	const Manifest & getManifest() const { return manifest; }

	/**
	 * Return a constant pointer to an Entry associated with the canonical
	 * Bucket name if it is in the Manifest map, null otherwise.
	 *
	 * @param name is a canonical Bucket name.
	 * @return a constant pointer to an Entry or null.
	 */
	virtual const Entry * find(const char * name) const;

	/**
	 * Reset this Action. This includes clearing the Manifest map.
	 *
	 * @return true if successful, false otherwise.
	 */
	virtual bool reset();

protected:

	/**
	 * This method is called when a service entry is returned by S3.
	 * Hayloft guarantees that this method does nothing except return
	 * ::S3StatusOK to indicate success. It can be safely overridden by the
	 * application for its own purposes. The overriding method does not need to
	 * call this method.
	 *
	 * @param ownerId is the AWS ID of the owner of the bucket.
	 * @param ownerDisplayName is the display name of the owner of the bucket.
	 * @param bucketName is the canonical name of the bucket.
	 * @param creationDateSeconds is the time and date when the bucket was
	 *        created represented as seconds since the UNIX epoch.
	 */
	virtual Status entry(const char * ownerId, const char * ownerDisplayName, const char * bucketName, Epochalseconds creationDateSeconds);

private:

	void initialize();

	void execute();

};

}
}
}

#endif
