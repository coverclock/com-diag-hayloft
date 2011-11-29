/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_REGION_
#define _H_COM_DIAG_HAYLOFT_S3_REGION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 *
 * The only way I can see to specify UNITED STATES CLASSIC (a.k.a. UNITED
 * STATES STANDARD) is to pass NULL to libs3 as a location constraint. This
 * causes libs3 to indicate UNITED STATES CLASSIC via invoking it as the
 * default by omitting the location constraint altogether. So although here we
 * encode UNITED STATES CLASSIC as an empty string, in practice it is mapped to
 * NULL when we use it in a libs3 call.
 */

#include <string>
#include "com/diag/hayloft/s3/types.h"
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Region {

public:

	static const char * REGION_ENV() { return "COM_DIAG_HAYLOFT_S3_REGION"; }

	static const char * ASIA_PACIFIC_NORTHEAST_1() { return "ap-northeast-1"; }

	static const char * ASIA_PACIFIC_SOUTHEAST_1() { return "ap-southeast-1"; }

	static const char * EUROPEAN_UNION_WEST_1() { return "eu-west-1"; }

	static const char * UNITED_STATES_STANDARD() { return ""; }

	static const char * UNITED_STATES_EAST_1() { return "us-east-1"; }

	static const char * UNITED_STATES_WEST_1() { return "us-west-1"; }

	static const char * UNITED_STATES_WEST_2() { return "us-west-2"; }

	static const char * DEFAULT() { return UNITED_STATES_STANDARD(); }

	static const size_t LENGTH = 64;

protected:

	std::string region;

public:

	explicit Region(
		const char * re = 0
	);

	virtual ~Region() {}

	const char * getRegion() const { return region.c_str(); }

	Region & setRegion(const char * re = 0) { region = set(re, REGION_ENV(), DEFAULT()); return *this; }

};

class RegionIreland : public Region {

public:

	explicit RegionIreland()
	: Region(EUROPEAN_UNION_WEST_1())
	{}

	virtual ~RegionIreland() {}

};

class RegionNorthernVirginia : public Region {

public:

	explicit RegionNorthernVirginia()
	: Region(UNITED_STATES_EAST_1())
	{}

	virtual ~RegionNorthernVirginia() {}

};

class RegionNorthernCalifornia : public Region {

public:

	explicit RegionNorthernCalifornia()
	: Region(UNITED_STATES_WEST_1())
	{}

	virtual ~RegionNorthernCalifornia() {}

};

class RegionOregon : public Region {

public:

	explicit RegionOregon()
	: Region(UNITED_STATES_WEST_2())
	{}

	virtual ~RegionOregon() {}

};

class RegionSingapore : public Region {

public:

	explicit RegionSingapore()
	: Region(ASIA_PACIFIC_SOUTHEAST_1())
	{}

	virtual ~RegionSingapore() {}

};

class RegionTokyo : public Region {

public:

	explicit RegionTokyo()
	: Region(ASIA_PACIFIC_NORTHEAST_1())
	{}

	virtual ~RegionTokyo() {}

};

class RegionUnitedStates : public Region {

public:

	explicit RegionUnitedStates()
	: Region(UNITED_STATES_STANDARD())
	{}

	virtual ~RegionUnitedStates() {}

};

}
}
}
}

#endif
