/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/s3/Object.h"
#include "com/diag/hayloft/s3/Bucket.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Show.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

Object::Object(const char * keyname, const Bucket & bucket)
: Container(bucket.getId(), bucket.getSecret(), bucket.getHostName(), bucket.getName(), bucket.getProtocol(), bucket.getStyle())
, key(keyname)
{}

Object::Object(const char * keyname, const Bucket & bucket, Multiplex & multiplex)
: Container(bucket.getId(), bucket.getSecret(), bucket.getHostName(), bucket.getName(), bucket.getProtocol(), bucket.getStyle(), multiplex)
, key(keyname)
{}

Object::~Object() {}

}
}
}
}
