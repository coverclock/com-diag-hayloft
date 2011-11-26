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
#include "com/diag/desperado/target.h"
#include "libs3.h"

namespace com {
namespace diag {
namespace hayloft {
namespace s3 {

class Endpoint {

public:

	/**
	 * AWS S3 places a limit of this many characters on the endpoint name (a.k.a.
	 * the host name).
	 */
	static const size_t LENGTH = S3_MAX_HOSTNAME_SIZE;

	static const char * ENDPOINT_ENV() { return "COM_DIAG_HAYLOFT_S3_ENDPOINT_HOSTNAME"; }

	static const char * ASIA_PACIFIC_NORTHEAST_1() { return "s3-ap-northeast-1.amazonaws.com"; }

	static const char * ASIA_PACIFIC_SOUTHEAST_1() { return "s3-ap-southeast-1.amazonaws.com"; }

	static const char * EUROPEAN_UNION_WEST_1() { return "s3-eu-west-1.amazonaws.com"; }

	static const char * UNITED_STATES_STANDARD() { return "s3.amazonaws.com"; }

	static const char * UNITED_STATES_EAST_1() { return "s3.amazonaws.com"; }

	static const char * UNITED_STATES_WEST_1() { return "s3-us-west-1.amazonaws.com"; }

	static const char * UNITED_STATES_WEST_2() { return "s3-us-west-2.amazonaws.com"; }

	static const char * DEFAULT() { return UNITED_STATES_STANDARD(); }

protected:

	std::string endpoint;

public:

	explicit Endpoint(
		const char * ep = 0
	);

	virtual ~Endpoint() {}

	const char * getEndpoint() const { return endpoint.c_str(); }

	Endpoint & setEndpoint(const char * ep = 0) { endpoint = set(ep, ENDPOINT_ENV(), DEFAULT()); return *this; }

};

class EndpointIreland : public Endpoint {

public:

	explicit EndpointIreland()
	: Endpoint(EUROPEAN_UNION_WEST_1())
	{}

	virtual ~EndpointIreland() {}

};

class EndpointNorthernVirginia : public Endpoint {

public:

	explicit EndpointNorthernVirginia()
	: Endpoint(UNITED_STATES_EAST_1())
	{}

	virtual ~EndpointNorthernVirginia() {}

};

class EndpointNorthernCalifornia : public Endpoint {

public:

	explicit EndpointNorthernCalifornia()
	: Endpoint(UNITED_STATES_WEST_1())
	{}

	virtual ~EndpointNorthernCalifornia() {}

};

class EndpointOregon : public Endpoint {

public:

	explicit EndpointOregon()
	: Endpoint(UNITED_STATES_WEST_2())
	{}

	virtual ~EndpointOregon() {}

};

class EndpointSingapore : public Endpoint {

public:

	explicit EndpointSingapore()
	: Endpoint(ASIA_PACIFIC_SOUTHEAST_1())
	{}

	virtual ~EndpointSingapore() {}

};

class EndpointTokyo : public Endpoint {

public:

	explicit EndpointTokyo()
	: Endpoint(ASIA_PACIFIC_NORTHEAST_1())
	{}

	virtual ~EndpointTokyo() {}

};

class EndpointUnitedStates : public Endpoint {

public:

	explicit EndpointUnitedStates()
	: Endpoint(UNITED_STATES_STANDARD())
	{}

	virtual ~EndpointUnitedStates() {}

};

}
}
}
}

#endif
