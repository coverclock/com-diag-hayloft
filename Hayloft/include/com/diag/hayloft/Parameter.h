/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_HAYLOFT_PARAMETER_H_
#define _COM_DIAG_HAYLOFT_PARAMETER_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace desperado {
class Input;
}
}
}

namespace com {
namespace diag {
namespace hayloft {

class Parameter {

private:

	std::string parameter;

public:

	explicit Parameter(const char * pa);

	explicit Parameter(const std::string & pa);

	explicit Parameter(::com::diag::desperado::Input & input, size_t maximum = intmaxof(size_t));

	explicit Parameter(::com::diag::desperado::Input * input /* TAKEN */, size_t maximum = intmaxof(size_t));

	virtual ~Parameter() {}

	operator const char *() const { return parameter.c_str(); }

	operator int() const { return parameter.length(); }

private:

	void source(::com::diag::desperado::Input & input, size_t maximum);

};

}
}
}

#endif
