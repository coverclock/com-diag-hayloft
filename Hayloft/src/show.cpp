/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/show.h"
#include "com/diag/hayloft/tostring.h"
#include "com/diag/hayloft/Credentials.h"
#include "com/diag/hayloft/Object.h"
#include "com/diag/hayloft/Bucket.h"
#include "com/diag/hayloft/ServiceManifest.h"
#include "com/diag/hayloft/BucketManifest.h"
#include "com/diag/hayloft/Grant.h"
#include "com/diag/hayloft/Log.h"
#include "com/diag/grandote/Seconds.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/types.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/hayloft/S3.h"

namespace com {
namespace diag {
namespace hayloft {

void show(const ::S3ResponseProperties * response, ::com::diag::grandote::MaskableLogger::Level level) {
	if (response != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			if (response->requestId != 0) { logger.log(level, "S3ResponseProperties@%p: requestId=\"%s\"\n", response, response->requestId); }
			if (response->requestId2 != 0) { logger.log(level, "S3ResponseProperties@%p: requestId2=\"%s\"\n", response, response->requestId2); }
			if (response->contentType != 0) { logger.log(level, "S3ResponseProperties@%p: contentType=\"%s\"\n", response, response->contentType); }
			if (response->contentLength > 0) { logger.log(level, "S3ResponseProperties@%p: contentLength=%llu\n", response, response->contentLength); }
			if (response->server != 0) { logger.log(level, "S3ResponseProperties@%p: server=\"%s\"\n", response, response->server); }
			if (response->eTag != 0) { logger.log(level, "S3ResponseProperties@%p: eTag=\"%s\"\n", response, response->eTag); }
			if (response->lastModified >= 0) { logger.log(level, "S3ResponseProperties@%p: lastModified=%lld\n", response, response->lastModified); }
			if (response->metaData != 0) {
				for (int ii = 0; ii < response->metaDataCount; ++ii) {
					if ((response->metaData[ii].name != 0) || (response->metaData[ii].value != 0)) {
						logger.log(level, "S3ResponseProperties@%p: \"%s\"=\"%s\"\n", response,
								(response->metaData[ii].name != 0) ? response->metaData[ii].name : "",
								(response->metaData[ii].value != 0) ? response->metaData[ii].value : "");
					}
				}
			}
		}
	}
}

void show(const ::S3ErrorDetails * details, ::com::diag::grandote::MaskableLogger::Level level) {
	if (details != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			if (details->message != 0) { logger.log(level, "S3ErrorDetails@%p: message=\"%s\"\n", details, details->message); }
			if (details->resource != 0) { logger.log(level, "S3ErrorDetails@%p: resource=\"%s\"\n", details, details->resource); }
			if (details->furtherDetails != 0) { logger.log(level, "S3ErrorDetails@%p: further=\"%s\"\n", details, details->furtherDetails); }
			if (details->extraDetails != 0) {
				for (int ii = 0; ii < details->extraDetailsCount; ++ii) {
					if ((details->extraDetails[ii].name != 0) || (details->extraDetails[ii].value != 0)) {
						logger.log(level, "S3ErrorDetails@%p: \"%s\"=\"%s\"\n", details,
								(details->extraDetails[ii].name != 0) ? details->extraDetails[ii].name : "",
								(details->extraDetails[ii].value != 0) ? details->extraDetails[ii].value : "");
					}
				}
			}
		}
	}
}

void show(const ::S3BucketContext * context, ::com::diag::grandote::MaskableLogger::Level level) {
	if (context != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			if (context->hostName != 0) { logger.log(level, "S3BucketContext@%p: hostName=\"%s\"\n", context, context->hostName); }
			if (context->bucketName != 0) { logger.log(level, "S3BucketContext@%p: bucketName=\"%s\"\n", context, context->bucketName); }
			logger.log(level, "S3BucketContext@%p: protocol=%d=\"%s\"\n", context, context->protocol, tostring(context->protocol));
			logger.log(level, "S3BucketContext@%p: uriStyle=%d=\"%s\"\n", context, context->uriStyle, tostring(context->uriStyle));
			if (context->accessKeyId != 0) { logger.log(level, "S3BucketContext@%p: accessKeyId=\"%s\"\n", context, Credentials::obfuscate(context->accessKeyId)); }
			if (context->secretAccessKey != 0) { logger.log(level, "S3BucketContext@%p: secretAccessKey=\"%s\"\n", context, Credentials::obfuscate(context->secretAccessKey)); }
		}
	}
}

void show(const ::S3PutProperties * properties, ::com::diag::grandote::MaskableLogger::Level level) {
	if (properties != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			if (properties->contentType != 0) { logger.log(level, "S3PutProperties@%p: contentType=\"%s\"\n", properties, properties->contentType); }
			if (properties->md5 != 0) { logger.log(level, "S3PutProperties@%p: md5=\"%s\"\n", properties, properties->md5); }
			if (properties->cacheControl != 0) { logger.log(level, "S3PutProperties@%p: cacheControl=\"%s\"\n", properties,  properties->cacheControl); }
			if (properties->contentDispositionFilename != 0) { logger.log(level, "S3PutProperties@%p: contentDispositionFilename=\"%s\"\n", properties, properties->contentDispositionFilename); }
			if (properties->contentEncoding != 0) { logger.log(level, "S3PutProperties@%p: contentEncoding=\"%s\"\n", properties, properties->contentEncoding); }
			if (properties->expires >= 0) { logger.log(level, "S3PutProperties@%p: expires=%lld\n", properties, properties->expires); }
			logger.log(level, "S3PutProperties@%p: cannedAcl=%d=\"%s\"\n", properties, properties->cannedAcl, tostring(properties->cannedAcl));
			if (properties->metaData != 0) {
				for (int ii = 0; ii < properties->metaDataCount; ++ii) {
					if ((properties->metaData[ii].name != 0) || (properties->metaData[ii].value != 0)) {
						logger.log(level, "S3PutProperties@%p: \"%s\"=\"%s\"\n", properties,
								(properties->metaData[ii].name != 0) ? properties->metaData[ii].name : "",
								(properties->metaData[ii].value != 0) ? properties->metaData[ii].value : "");
					}
				}
			}
		}
	}
}

void show(const ::S3GetConditions * conditions, ::com::diag::grandote::MaskableLogger::Level level) {
	if (conditions != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			if (conditions->ifModifiedSince >= 0) { logger.log(level, "S3GetConditions@%p: ifModifiedSince=%lld\n", conditions, conditions->ifModifiedSince); }
			if (conditions->ifNotModifiedSince >= 0) { logger.log(level, "S3GetConditions@%p: ifNotModifiedSince=%lld\n", conditions, conditions->ifNotModifiedSince); }
			if (conditions->ifMatchETag != 0) { logger.log(level, "S3GetConditions@%p: ifMatchETag=\"%s\"\n", conditions, conditions->ifMatchETag); }
			if (conditions->ifNotMatchETag != 0) { logger.log(level, "S3GetConditions@%p: ifNotMatchETag=\"%s\"\n", conditions, conditions->ifNotMatchETag); }
		}
	}
}

void show(const ::S3AclGrant * grant, int count, ::com::diag::grandote::MaskableLogger::Level level) {
	if (grant != 0) {
		::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
		if (logger.isEnabled(level)) {
			for (int ii = 0; ii < count; ++ii) {
				switch (grant->granteeType) {
				case ::S3GranteeTypeAmazonCustomerByEmail:
					logger.log(level, "S3AclGrant@%p: granteeType=%d=\"%s\" permission=%d=\"%s\" emailAddress=\"%.*s\"\n",
						grant,
						grant->granteeType, tostring(grant->granteeType),
						grant->permission, tostring(grant->permission),
						sizeof(grant->grantee.amazonCustomerByEmail.emailAddress), grant->grantee.amazonCustomerByEmail.emailAddress
					);
					break;
				case ::S3GranteeTypeCanonicalUser:
					logger.log(level, "S3AclGrant@%p: granteeType=%d=\"%s\" permission=%d=\"%s\" id=\"%.*s\" displayName=\"%.*s\"\n",
						grant,
						grant->granteeType, tostring(grant->granteeType),
						grant->permission, tostring(grant->permission),
						sizeof(grant->grantee.canonicalUser.id), grant->grantee.canonicalUser.id,
						sizeof(grant->grantee.canonicalUser.displayName), grant->grantee.canonicalUser.displayName
					);
					break;
				default:
					logger.log(level, "S3AclGrant@%p: granteeType=%d=\"%s\" permission=%d=\"%s\"\n",
						grant,
						grant->granteeType, tostring(grant->granteeType),
						grant->permission, tostring(grant->permission)
					);
					break;
				}
				++grant;
			}
		}
	}
}

void show(const Object & object, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		std::string uri;
		logger.log(level, "Object@%p: uri=\"%s\"\n", &object, tostring(object, uri));
		// Object
		if ((object.getKey())[0] != '\0') { logger.log(level, "Object@%p: key=\"%s\"\n", &object, object.getKey()); }
		if ((object.getContentType())[0] != '\0') { logger.log(level, "Object@%p: contentType=\"%s\"\n", &object, object.getContentType()); }
		if ((object.getETag())[0] != '\0') { logger.log(level, "Object@%p: eTag=\"%s\"\n", &object, object.getETag()); }
		logger.log(level, "Object@%p: contentLength=%llu\n", &object, object.getContentLength());
		Epochalseconds modified = object.getModificationTime();
		if (modified >= 0) {
			::com::diag::grandote::Seconds seconds;
			logger.log(level, "Object@%p: modificationTime=%lld=\"%s\"\n", &object, modified, seconds.zulu(modified));
		} else {
			logger.log(level, "Object@%p: modificationTime=%lld\n", &object, modified);
		}
		Object::Metadata::const_iterator here = object.getMetadata().begin();
		Object::Metadata::const_iterator there = object.getMetadata().end();
		while (here != there) {
			if ((object.getKey())[0] != '\0') { logger.log(level, "Object@%p: \"%s\"=\"%s\"\n", &object, here->first.c_str(), here->second.c_str()); }
			++here;
		}
		//Container
		if ((object.getCanonical())[0] != '\0') { logger.log(level, "Object@%p: canonical=\"%s\"\n", &object, object.getCanonical()); }
		logger.log(level, "Object@%p: style=%d=\"%s\"\n", &object, object.getStyle(), tostring(object.getStyle()));
		// Service
		if ((object.getId())[0] != '\0') { logger.log(level, "Object@%p: id=\"%s\"\n", &object, Credentials::obfuscate(object.getId())); }
		if ((object.getSecret())[0] != '\0') { logger.log(level, "Object@%p: secret=\"%s\"\n", &object, Credentials::obfuscate(object.getSecret())); }
		if ((object.getEndpoint())[0] != '\0') { logger.log(level, "Object@%p: endpoint=\"%s\"\n", &object, object.getEndpoint()); }
		logger.log(level, "Object@%p: prototol=%d=\"%s\"\n", &object, object.getProtocol(), tostring(object.getProtocol()));
		// Action
		if ((object.getServer())[0] != '\0') { logger.log(level, "Object@%p: server=\"%s\"\n", &object, object.getServer()); }
		if ((object.getRequestId())[0] != '\0') { logger.log(level, "Object@%p: requestId=\"%s\"\n", &object, object.getRequestId()); }
		if ((object.getRequestId2())[0] != '\0') { logger.log(level, "Object@%p: requestId2=\"%s\"\n", &object, object.getRequestId2()); }
		logger.log(level, "Object@%p: status=%d=\"%s\"\n", &object, object.getStatus(), tostring(object.getStatus()));
	}
}

void show(const Bucket & bucket, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		std::string uri;
		logger.log(level, "Bucket@%p: uri=\"%s\"\n", &bucket, tostring(bucket, uri));
		// Bucket
		if ((bucket.getName())[0] != '\0') { logger.log(level, "Bucket@%p: name=\"%s\"\n", &bucket, bucket.getName()); }
		if ((bucket.getRegion())[0] != '\0') { logger.log(level, "Bucket@%p: region=\"%s\"\n", &bucket, bucket.getRegion()); }
		logger.log(level, "Bucket@%p: access=%d=\"%s\"\n", &bucket, bucket.getAccess(), tostring(bucket.getAccess()));
		//Container
		if ((bucket.getCanonical())[0] != '\0') { logger.log(level, "Bucket@%p: canonical=\"%s\"\n", &bucket, bucket.getCanonical()); }
		logger.log(level, "Bucket@%p: style=%d=\"%s\"\n", &bucket, bucket.getStyle(), tostring(bucket.getStyle()));
		// Service
		if ((bucket.getId())[0] != '\0') { logger.log(level, "Bucket@%p: id=\"%s\"\n", &bucket, Credentials::obfuscate(bucket.getId())); }
		if ((bucket.getSecret())[0] != '\0') { logger.log(level, "Bucket@%p: secret=\"%s\"\n", &bucket, Credentials::obfuscate(bucket.getSecret())); }
		if ((bucket.getEndpoint())[0] != '\0') { logger.log(level, "Bucket@%p: endpoint=\"%s\"\n", &bucket, bucket.getEndpoint()); }
		logger.log(level, "Bucket@%p: prototol=%d=\"%s\"\n", &bucket, bucket.getProtocol(), tostring(bucket.getProtocol()));
		// Action
		if ((bucket.getServer())[0] != '\0') { logger.log(level, "Bucket@%p: server=\"%s\"\n", &bucket, bucket.getServer()); }
		if ((bucket.getRequestId())[0] != '\0') { logger.log(level, "Bucket@%p: requestId=\"%s\"\n", &bucket, bucket.getRequestId()); }
		if ((bucket.getRequestId2())[0] != '\0') { logger.log(level, "Bucket@%p: requestId2=\"%s\"\n", &bucket, bucket.getRequestId2()); }
		logger.log(level, "Object@%p: status=%d=\"%s\"\n", &bucket, bucket.getStatus(), tostring(bucket.getStatus()));
	}
}

void show(const ServiceManifest & manifest, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		ServiceManifest::Manifest::const_iterator here = manifest.getManifest().begin();
		ServiceManifest::Manifest::const_iterator there = manifest.getManifest().end();
		const char * canonical;
		const char * owner;
		const char * display;
		Epochalseconds created;
		::com::diag::grandote::Seconds seconds;
		while (here != there) {
			canonical = here->second.getCanonical();
			owner = here->second.getOwnerId();
			display = here->second.getOwnerDisplayName();
			created = here->second.getCreated();
			logger.log(level, "ServiceManifest@%p: canonical=\"%s\" owner=\"%s\" display=\"%s\" created=%lld=\"%s\"\n", &manifest, canonical, owner, display, created, seconds.zulu(created));
			++here;
		}
	}
}

void show(const BucketManifest & manifest, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		{
			BucketManifest::Manifest::const_iterator here = manifest.getManifest().begin();
			BucketManifest::Manifest::const_iterator there = manifest.getManifest().end();
			const char * key;
			Epochalseconds modified;
			const char * etag;
			Octets size;
			const char * owner;
			const char * display;
			::com::diag::grandote::Seconds seconds;
			while (here != there) {
				key = here->second.getKey();
				modified = here->second.getModified();
				etag = here->second.getETag();
				size = here->second.getSize();;
				owner = here->second.getOwnerId();
				display = here->second.getOwnerDisplayName();
				logger.log(level, "BucketManifest@%p: key=\"%s\" etag=\"%s\" size=%llu owner=\"%s\" display=\"%s\" modified=%lld=\"%s\"\n", &manifest, key, etag, size, owner, display, modified, seconds.zulu(modified));
				++here;
			}
		}
		{
			BucketManifest::Common::const_iterator here = manifest.getCommon().begin();
			BucketManifest::Common::const_iterator there = manifest.getCommon().begin();
			while (here != there) {
				logger.log(level, "BucketManifest@%p: common=\"%s\"\n", here->c_str());
				++here;
			}
		}
	}
}

void show(const Grant & grant, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		if (grant.getKeyPointer() != 0) { logger.log(level, "Grant@%p: key=\"%s\"\n", &grant, grant.getKey()); }
		logger.log(level, "Grant@%p: canonical=\"%s\"\n", &grant, grant.getCanonical());
		logger.log(level, "Grant@%p: owner=\"%s\"\n", &grant, grant.getOwnerId());
		logger.log(level, "Grant@%p: display=\"%s\"\n", &grant, grant.getOwnerDisplayName());
		Grant::List::const_iterator here = grant.getAccessControlList().begin();
		Grant::List::const_iterator there = grant.getAccessControlList().end();
		while (here != there) {
			::S3GranteeType granteeType = here->getGranteeType();
			::S3Permission permission = here->getPermission();
			switch (granteeType) {
			case ::S3GranteeTypeAmazonCustomerByEmail:
				logger.log(level, "Grant@%p: type=%d=\"%s\" permission=%d=\"%s\" email=\"%s\"\n", &grant, granteeType, tostring(granteeType), permission, tostring(permission), here->getOwnerIdOrEmailAddress());
				break;
			case ::S3GranteeTypeCanonicalUser:
				logger.log(level, "Grant@%p: type=%d=\"%s\" permission=%d=\"%s\" owner=\"%s\" display=\"%s\"\n", &grant, granteeType, tostring(granteeType), permission, tostring(permission), here->getOwnerIdOrEmailAddress(), here->getOwnerDisplayName());
				break;
			default:
				logger.log(level, "Grant@%p: type=%d=\"%s\" permission=%d=\"%s\"\n", &grant, granteeType, tostring(granteeType), permission, tostring(permission));
				break;
			}
			++here;
		}
	}
}

void show(const Log & log, ::com::diag::grandote::MaskableLogger::Level level) {
	::com::diag::grandote::MaskableLogger & logger = ::com::diag::grandote::MaskableLogger::instance();
	if (logger.isEnabled(level)) {
		logger.log(level, "Log@%p: target=\"%s\"\n", &log, log.getTarget());
		logger.log(level, "Log@%p: prefix=\"%s\"\n", &log, log.getPrefix());
		const Grant & grant = log;
		show(grant);
	}
}

}
}
}
