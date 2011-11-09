/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_LOCATIONCONSTRAINT
#define _H_COM_DIAG_HAYLOFT_S3_LOCATIONCONSTRAINT

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

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class LocationConstraint {

private:

	std::string constraint;

public:

	static const char * DEFAULT() { return ""; }

	static const size_t LENGTH = 64;

	explicit LocationConstraint(const char * locationconstraint = DEFAULT());

	virtual ~LocationConstraint() {}

	const char * getLocationConstraint() const { return constraint.c_str(); }

	size_t getLength() const { return constraint.length(); }

};

class LocationConstraintEuropeanUnion : public LocationConstraint {

public:

	explicit LocationConstraintEuropeanUnion()
	: LocationConstraint("EU")
	{}

	virtual ~LocationConstraintEuropeanUnion() {}

};

class LocationConstraintUnitedStatesWest1 : public LocationConstraint {

public:

	explicit LocationConstraintUnitedStatesWest1()
	: LocationConstraint("us-west-1")
	{}

	virtual ~LocationConstraintUnitedStatesWest1() {}

};

class LocationConstraintAsiaPacificSouthEast1 : public LocationConstraint {

public:

	explicit LocationConstraintAsiaPacificSouthEast1()
	: LocationConstraint("ap-southeast-1")
	{}

	virtual ~LocationConstraintAsiaPacificSouthEast1() {}

};

class LocationConstraintAsiaPacificNorthEast1 : public LocationConstraint {

public:

	explicit LocationConstraintAsiaPacificNorthEast1()
	: LocationConstraint("ap-northeast-1")
	{}

	virtual ~LocationConstraintAsiaPacificNorthEast1() {}

};

class LocationConstraintUnitedStatesClassic : public LocationConstraint {

public:

	explicit LocationConstraintUnitedStatesClassic()
	: LocationConstraint("")
	{}

	virtual ~LocationConstraintUnitedStatesClassic() {}

};

}
}
}
}

#endif
