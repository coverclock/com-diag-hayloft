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

	static const char * ASIA_PACIFIC_NORTHEAST_1() { return "ap-northeast-1"; }
	static const char * ASIA_PACIFIC_SOUTHEAST_1() { return "ap-southeast-1"; }
	static const char * EUROPEAN_UNION_WEST_1() { return "eu-west-1"; }
	static const char * UNITED_STATES_CLASSIC() { return ""; }
	static const char * UNITED_STATES_EAST_1() { return "us-east-1"; }
	static const char * UNITED_STATES_WEST_1() { return "us-west-1"; }
	static const char * UNITED_STATES_WEST_2() { return "us-west-2"; }

	static const char * DEFAULT() { return UNITED_STATES_CLASSIC(); }

	static const size_t LENGTH = 64;

	explicit LocationConstraint(const char * lo = DEFAULT());

	virtual ~LocationConstraint() {}

	const char * getConstraint() const { return constraint.c_str(); }

	size_t getLength() const { return constraint.length(); }

};

class LocationConstraintIreland : public LocationConstraint {

public:

	explicit LocationConstraintIreland()
	: LocationConstraint(EUROPEAN_UNION_WEST_1())
	{}

	virtual ~LocationConstraintIreland() {}

};

class LocationConstraintNorthernVirginia : public LocationConstraint {

public:

	explicit LocationConstraintNorthernVirginia()
	: LocationConstraint(UNITED_STATES_EAST_1())
	{}

	virtual ~LocationConstraintNorthernVirginia() {}

};

class LocationConstraintNorthernCalifornia : public LocationConstraint {

public:

	explicit LocationConstraintNorthernCalifornia()
	: LocationConstraint(UNITED_STATES_WEST_1())
	{}

	virtual ~LocationConstraintNorthernCalifornia() {}

};

class LocationConstraintOregon : public LocationConstraint {

public:

	explicit LocationConstraintOregon()
	: LocationConstraint(UNITED_STATES_WEST_2())
	{}

	virtual ~LocationConstraintOregon() {}

};

class LocationConstraintSingapore : public LocationConstraint {

public:

	explicit LocationConstraintSingapore()
	: LocationConstraint(ASIA_PACIFIC_SOUTHEAST_1())
	{}

	virtual ~LocationConstraintSingapore() {}

};

class LocationConstraintTokyo : public LocationConstraint {

public:

	explicit LocationConstraintTokyo()
	: LocationConstraint(ASIA_PACIFIC_NORTHEAST_1())
	{}

	virtual ~LocationConstraintTokyo() {}

};

class LocationConstraintUnitedStates : public LocationConstraint {

public:

	explicit LocationConstraintUnitedStates()
	: LocationConstraint(UNITED_STATES_CLASSIC())
	{}

	virtual ~LocationConstraintUnitedStates() {}

};

}
}
}
}

#endif
