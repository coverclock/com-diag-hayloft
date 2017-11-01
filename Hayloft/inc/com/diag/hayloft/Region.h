/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_REGION_
#define _H_COM_DIAG_HAYLOFT_REGION_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "com/diag/hayloft/set.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Region specifies on what geographical collection of AWS servers an S3 bucket
 * will be hosted. This is known in some contexts as a Location Constraint.
 * Buckets are hosted in specific geographic areas for reasons of performance,
 * latency, redundancy, and most especially for legal reasons such as national
 * privacy laws.
 */
class Region {

public:

	/**
	 * This is the name of the environmental variable with which you can
	 * specify the name of your default region.
	 */
	static const char * REGION_ENV() { return "COM_DIAG_HAYLOFT_S3_REGION_NAME"; }

	static const char * ASIA_PACIFIC_NORTHEAST_1() { return "ap-northeast-1"; }

	static const char * ASIA_PACIFIC_SOUTHEAST_1() { return "ap-southeast-1"; }

	static const char * EUROPEAN_UNION_WEST_1() { return "eu-west-1"; }

	/**
	 * The only way I can see to specify UNITED STATES STANDARD (a.k.a. UNITED
	 * STATES CLASSIC) is to pass NULL to libs3 as a location constraint. This
	 * causes libs3 to indicate UNITED STATES STANDARD via invoking it as the
	 * default by omitting the location constraint altogether. So although here
	 * we encode UNITED STATES STANDARD as an empty string, in practice it is
	 * mapped to NULL when we use it in a libs3 call.
	 *
	 * UNITED STATES STANDARD is a actually a bi-coastal region, using servers
	 * on both the East and West coasts. As such it exhibits more artifacts of
	 * the AWS Eventual Consistency architecture than the other regions. In
	 * fact, the consistency rules for some of the other reasons are more
	 * stringent than that for UNITED STATES STANDARD, guaranteeing, for
	 * example, read-after-write consistency, where as the bi-coastal region
	 * does not.
	 */
	static const char * UNITED_STATES_STANDARD() { return ""; }

	static const char * UNITED_STATES_EAST_1() { return "us-east-1"; }

	static const char * UNITED_STATES_WEST_1() { return "us-west-1"; }

	static const char * UNITED_STATES_WEST_2() { return "us-west-2"; }

	static const char * SOUTH_AMERICA_EAST_1() { return "sa-east-1"; }

	/**
	 * This is the name of the default region if you don't specify one
	 * otherwise. UNITED STATES STANDARD is chosen as the default because it
	 * is still the most commonly used region, probably because it has the
	 * least expensive billing rates.
	 */
	static const char * DEFAULT() { return UNITED_STATES_STANDARD(); }

protected:

	std::string region;

public:

	/**
	 * Ctor.
	 *
	 * @param re is an AWS region name. If null, the region name is
	 *        taken from the environment. A copy is made of this C string.
	 */
	explicit Region(
		const char * re = 0
	);

	/**
	 * Dtor.
	 */
	virtual ~Region() {}

	/**
	 * Get the region name.
	 *
	 * @return the region name.
	 */
	const char * getRegion() const { return region.c_str(); }

	/**
	 * Set the region name.
	 *
	 * @param re is an AWS region name. If null, the region name is
	 *        taken from the environment. A copy is made of this C string.
	 * @return a reference to this object.
	 */
	Region & setRegion(const char * re = 0) { region = set(re, REGION_ENV(), DEFAULT()); return *this; }

};

/**
 * RegionIreland is a Region that represents the Ireland (a.k.a EU WEST 1)
 * AWS region.
 */
class RegionIreland : public Region {

public:

	explicit RegionIreland()
	: Region(EUROPEAN_UNION_WEST_1())
	{}

	virtual ~RegionIreland() {}

};

/**
 * RegionNorthernVirginia is a Region that represents the Northern
 * Virginia (a.k.a. EAST 1) AWS region.
 */
class RegionNorthernVirginia : public Region {

public:

	explicit RegionNorthernVirginia()
	: Region(UNITED_STATES_EAST_1())
	{}

	virtual ~RegionNorthernVirginia() {}

};

/**
 * RegionNorthernCalifornia is a Region that represents the Northern
 * California (a.k.a. WEST 1) AWS region.
 */
class RegionNorthernCalifornia : public Region {

public:

	explicit RegionNorthernCalifornia()
	: Region(UNITED_STATES_WEST_1())
	{}

	virtual ~RegionNorthernCalifornia() {}

};

/**
 * RegionOregon is a Region that represents the Oregon (a.k.a. WEST 2)
 * AWS region.
 */
class RegionOregon : public Region {

public:

	explicit RegionOregon()
	: Region(UNITED_STATES_WEST_2())
	{}

	virtual ~RegionOregon() {}

};

/**
 * RegionSingapore is a Region that represents the Singapore (a.k.a.
 * SOUTH EAST 1) AWS region.
 */
class RegionSingapore : public Region {

public:

	explicit RegionSingapore()
	: Region(ASIA_PACIFIC_SOUTHEAST_1())
	{}

	virtual ~RegionSingapore() {}

};

/**
 * RegionTokyo is a Region that represents the Tokyo (a.k.a.
 * NORTH EAST 1) AWS region.
 */
class RegionTokyo : public Region {

public:

	explicit RegionTokyo()
	: Region(ASIA_PACIFIC_NORTHEAST_1())
	{}

	virtual ~RegionTokyo() {}

};

/**
 * RegionUnitedStates is a Region that represents the bi-coastal
 * United States (a.k.a. STANDARD or CLASSIC) AWS region.
 */
class RegionUnitedStates : public Region {

public:

	explicit RegionUnitedStates()
	: Region(UNITED_STATES_STANDARD())
	{}

	virtual ~RegionUnitedStates() {}

};

/**
 * RegionSaoPaulo is a Region that represents the Sao Paulo (a.k.a.
 * SOUTH AMERICA EAST 1) AWS region.
 */
class RegionSaoPaulo : public Region {

public:

	explicit RegionSaoPaulo()
	: Region(SOUTH_AMERICA_EAST_1())
	{}

	virtual ~RegionSaoPaulo() {}

};

}
}
}

#endif
