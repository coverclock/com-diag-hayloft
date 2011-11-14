/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/Properties.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Properties::Properties(
	const char * ct,
	const char * m5,
	const char * cc,
	const char * cd,
	const char * ce,
	const int64_t ex,
	const Access & ac,
	const Metadata & md
)
: contentType((ct != 0) ? ct : "")
, md5((m5 != 0) ? m5 : "")
, cacheControl((cc != 0) ? cc : "")
, contentDispositionFilename((cd != 0) ? cd : "")
, contentEncoding((ce != 0) ? ce : "")
{
	properties.contentType = contentType.empty() ? 0 : contentType.c_str();
	properties.md5 = md5.empty() ? 0 : md5.c_str();
	properties.cacheControl = cacheControl.empty() ? 0 : cacheControl.c_str();
	properties.contentDispositionFilename = contentDispositionFilename.empty() ? 0 : contentDispositionFilename.c_str();
	properties.contentEncoding = contentEncoding.empty() ? 0 : contentEncoding.c_str();
	properties.expires = ex;
	properties.cannedAcl = ac.getAccess();
	properties.metaDataCount = md.size();
	if (properties.metaDataCount > 0) {
		Metadata::const_iterator here;
		Metadata::const_iterator end;
		for (here = md.begin(), end = md.end(); here != end; ++here) {
			metadata.insert(Pair(here->first, here->second));
		}
		::S3NameValue * pair = new ::S3NameValue [properties.metaDataCount];
		properties.metaData = pair;
		for (here = metadata.begin(), end = metadata.end(); here != end; ++here, ++pair) {
			pair->name = (here->first).c_str();
			pair->value = (here->second).c_str();
		}
	} else {
		properties.metaData = 0;
	}
	show(&properties, Logger::DEBUG);
}

Properties::~Properties() {
	if (properties.metaData != 0) {
		delete [] properties.metaData;
	}
}

void show(const ::S3PutProperties * properties, Logger::Level level) {
	Logger & logger = Logger::instance();
	if (logger.isEnabled(level)) {
		logger.log(level, "Properties@%p: contentType=\"%s\"\n", properties, (properties->contentType != 0) ? properties->contentType : "");
		logger.log(level, "Properties@%p: md5=\"%s\"\n", properties, (properties->md5 != 0) ? properties->md5 : "");
		logger.log(level, "Properties@%p: cacheControl=\"%s\"\n", properties, (properties->cacheControl != 0) ? properties->cacheControl : "");
		logger.log(level, "Properties@%p: contentDispositionFilename=\"%s\"\n", properties, (properties->contentDispositionFilename != 0) ? properties->contentDispositionFilename : "");
		logger.log(level, "Properties@%p: contentEncoding=\"%s\"\n", properties, (properties->contentEncoding != 0) ? properties->contentEncoding : "");
		logger.log(level, "Properties@%p: expires=%lld\n", properties, properties->expires);
		logger.log(level, "Properties@%p: cannedAcl=%d\n", properties, properties->cannedAcl);
		logger.log(level, "Properties@%p: metaDataCount=%d\n", properties, properties->metaDataCount);
		for (int ii = 0; ii < properties->metaDataCount; ++ii) {
			if ((properties->metaData[ii].name != 0) || (properties->metaData[ii].value != 0)) {
				logger.log(level, "Properties@%p: \"%s\"=\"%s\"\n", properties,
						(properties->metaData[ii].name != 0) ? properties->metaData[ii].name : "",
						(properties->metaData[ii].value != 0) ? properties->metaData[ii].value : "");
			}
		}
	}
}

}
}
}
}
