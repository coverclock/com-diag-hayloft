/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_S3_ENDPOINT_
#define _H_COM_DIAG_HAYLOFT_S3_ENDPOINT_

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
#include "com/diag/hayloft/types.h"
#include "com/diag/hayloft/s3/S3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

/**
 * Endpoint identifies a particular host name for use in the Uniform Resource
 * Identifier for executing Actions or accessing Services. The end point you
 * should use depends on your geographic location and the region or location
 * constraint you have used or intend to use with any S3 buckets. S3 rejects
 * operations where the end point doesn't jive with the location constraint.
 * Your default end point (and region) can be set with environmental variables.
 * Be aware that AWS occasionally adds new end points and regions.
 */
class Endpoint {

public:

	/**
	 * This is the name of the environmental variable with which you can
	 * specify the name of your default end point.
	 */
	static const char * ENDPOINT_ENV() { return "COM_DIAG_HAYLOFT_S3_ENDPOINT_HOSTNAME"; }

	static const char * ASIA_PACIFIC_NORTHEAST_1() { return "s3-ap-northeast-1.amazonaws.com"; }

	static const char * ASIA_PACIFIC_SOUTHEAST_1() { return "s3-ap-southeast-1.amazonaws.com"; }

	static const char * EUROPEAN_UNION_WEST_1() { return "s3-eu-west-1.amazonaws.com"; }

	static const char * UNITED_STATES_STANDARD() { return "s3.amazonaws.com"; }

	static const char * UNITED_STATES_EAST_1() { return "s3.amazonaws.com"; }

	static const char * UNITED_STATES_WEST_1() { return "s3-us-west-1.amazonaws.com"; }

	static const char * UNITED_STATES_WEST_2() { return "s3-us-west-2.amazonaws.com"; }

	static const char * SOUTH_AMERICA_EAST_1() { return "s3-sa-east-1.amazonaws.com"; }

	/**
	 * This is the name of the default end point if you don't specify one
	 * otherwise.
	 */
	static const char * DEFAULT() { return UNITED_STATES_STANDARD(); }

protected:

	std::string endpoint;

public:

	/**
	 * Ctor.
	 *
	 * @param ep is an AWS end point name. If null, the end point name is
	 *        taken from the environment. A copy is made of this C string.
	 */
	explicit Endpoint(
		const char * ep = 0
	);

	/**
	 * Dtor.
	 */
	virtual ~Endpoint() {}

	/**
	 * Get the end point name.
	 *
	 * @return the end point name.
	 */
	const char * getEndpoint() const { return endpoint.c_str(); }

	/**
	 * Set the end point name.
	 *
	 * @param ep is an AWS end point name. If null, the end point name is
	 *        taken from the environment.
	 * @return a reference to this object.
	 */
	Endpoint & setEndpoint(const char * ep = 0) { endpoint = set(ep, ENDPOINT_ENV(), DEFAULT()); return *this; }

};

/**
 * EndpointIreland is an Endpoint that represents the Ireland (a.k.a. EU WEST 1)
 * AWS region.
 */
class EndpointIreland : public Endpoint {

public:

	explicit EndpointIreland()
	: Endpoint(EUROPEAN_UNION_WEST_1())
	{}

	virtual ~EndpointIreland() {}

};

/**
 * EndpointNorthernVirginia is an Endpoint that represents the Northern
 * Virginia (a.k.a. EAST 1) AWS region.
 */
class EndpointNorthernVirginia : public Endpoint {

public:

	explicit EndpointNorthernVirginia()
	: Endpoint(UNITED_STATES_EAST_1())
	{}

	virtual ~EndpointNorthernVirginia() {}

};

/**
 * EndpointNorthernCalifornia is an Endpoint that represents the Northern
 * California (a.k.a. WEST 1) AWS region.
 */
class EndpointNorthernCalifornia : public Endpoint {

public:

	explicit EndpointNorthernCalifornia()
	: Endpoint(UNITED_STATES_WEST_1())
	{}

	virtual ~EndpointNorthernCalifornia() {}

};

/**
 * EndpointOregon is an Endpoint that represents the Oregon (a.k.a. WEST 2)
 * AWS region.
 */
class EndpointOregon : public Endpoint {

public:

	explicit EndpointOregon()
	: Endpoint(UNITED_STATES_WEST_2())
	{}

	virtual ~EndpointOregon() {}

};

/**
 * EndpointSingapore is an Endpoint that represents the Singapore (a.k.a.
 * SOUTH EAST 1) AWS region.
 */
class EndpointSingapore : public Endpoint {

public:

	explicit EndpointSingapore()
	: Endpoint(ASIA_PACIFIC_SOUTHEAST_1())
	{}

	virtual ~EndpointSingapore() {}

};

/**
 * EndpointTokyo is an Endpoint that represents the Tokyo (a.k.a.
 * NORTH EAST 1) AWS region.
 */
class EndpointTokyo : public Endpoint {

public:

	explicit EndpointTokyo()
	: Endpoint(ASIA_PACIFIC_NORTHEAST_1())
	{}

	virtual ~EndpointTokyo() {}

};

/**
 * EndpointUnitedStates is an Endpoint that represents the bi-coastal
 * United States (a.k.a. STANDARD or CLASSIC) AWS region.
 */
class EndpointUnitedStates : public Endpoint {

public:

	explicit EndpointUnitedStates()
	: Endpoint(UNITED_STATES_STANDARD())
	{}

	virtual ~EndpointUnitedStates() {}

};

/**
 * EndpointSaoPaulo is an Endpoint that represents the Sao Paulo (a.k.a.
 * SOUTH AMERICA EAST 1) AWS region.
 */
class EndpointSaoPaulo : public Endpoint {

public:

	explicit EndpointSaoPaulo()
	: Endpoint(SOUTH_AMERICA_EAST_1())
	{}

	virtual ~EndpointSaoPaulo() {}

};

}
}
}
}

#endif
