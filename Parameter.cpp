/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Parameter.h"
#include "com/diag/desperado/Input.h"

namespace com {
namespace diag {
namespace hayloft {

Parameter::Parameter(const char * pa)
: parameter(pa)
{}

Parameter::Parameter(const std::string & pa)
: parameter(pa)
{}

Parameter::Parameter(::com::diag::desperado::Input & input, size_t maximum) {
	source(input, maximum);
}

Parameter::Parameter(::com::diag::desperado::Input * input /* TAKEN */, size_t maximum) {
	if (input != 0) {
		source(*input, maximum);
		delete input;
	}
}

void Parameter::source(::com::diag::desperado::Input & input, size_t maximum) {
	int ch;
	while (maximum > 0) {
		ch = input();
		if ((ch == EOF) || (ch == '\0') || (ch == '\n')) { break; }
		parameter += ch;
		--maximum;
	}
}

}
}
}
