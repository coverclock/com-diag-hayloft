/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/s3/GrantXml.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

GrantXml::GrantXml(const char * xmlstring)
: Grant()
, xml(xmlstring)
{
	initialize();
	execute();
}

GrantXml::~GrantXml() {
}

void GrantXml::initialize() {
	grants = new ::S3AclGrant [COUNT];
}

void GrantXml::execute() {
	char ownerid[OWNER_LEN];
	char ownerdisplayname[DISPLAY_LEN];
	// libs3 has a minor flaw (the only one I've found): S3_convert_acl()
	// doesn't declare the XML string const. But it can be const, because
	// it is merely passed to XML2's simplexml_add() which _does_ declare it
	// const. So I cheat here and hope it never comes back to bite me.
	status = S3_convert_acl(
		const_cast<char *>(xml.c_str()),
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
	}
}

}
}
}
}
