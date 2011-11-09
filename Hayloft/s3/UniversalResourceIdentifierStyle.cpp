/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/UniversalResourceIdentifierStyle.h"
#include "com/diag/hayloft/Logger.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

const ::S3UriStyle UniversalResourceIdentifierStyle::DEFAULT;

UniversalResourceIdentifierStyle::UniversalResourceIdentifierStyle(::S3UriStyle uri)
: style(uri)
{
	Logger::instance().debug("UniversalResourceIdentifierStyle@%p: uristyle=%d\n", this, style);
}

}
}
}
}
