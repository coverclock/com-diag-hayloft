/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/hayloft/Seconds.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/LocalTime.h"
#include "com/diag/desperado/Epoch.h"

namespace com {
namespace diag {
namespace hayloft {

unsigned int Seconds::dontcare;

Seconds::Seconds()
: numerator(0)
, denominator(0)
{
	::com::diag::desperado::Platform::instance().frequency(numerator, denominator);
}

Seconds::~Seconds()
{}

const char * Seconds::zulu(Epochalseconds seconds, unsigned int & year, unsigned int & month, unsigned int & day, unsigned int & hour, unsigned int & minute, unsigned int & second) {
	ticks_t ticks = (seconds * numerator) / denominator;
	::com::diag::desperado::CommonEra commonera(0);
	commonera.fromTicks(ticks);
	year = commonera.getYear();
	month = commonera.getMonth();
	day = commonera.getDay();
	hour = commonera.getHour();
	minute = commonera.getMinute();
	second = commonera.getSecond();
	return timestamp.iso8601(commonera);
}

const char * Seconds::juliet(Epochalseconds seconds, unsigned int & year, unsigned int & month, unsigned int & day, unsigned int & hour, unsigned int & minute, unsigned int & second) {
	ticks_t ticks = (seconds * numerator) / denominator;
	::com::diag::desperado::CommonEra commonera(0);
	commonera.fromTicks(ticks);
	::com::diag::desperado::LocalTime localtime;
	localtime.fromCommonEra(commonera);
	year = localtime.getYear();
	month = localtime.getMonth();
	day = localtime.getDay();
	hour = localtime.getHour();
	minute = localtime.getMinute();
	second = localtime.getSecond();
	return timestamp.iso8601(localtime);
}

Epochalseconds Seconds::now() const {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	::com::diag::desperado::CommonEra commonera(0);
	commonera.fromNow();
	seconds_t commoneraseconds = platform.getLeapSecondTicks() ? commonera.toSeconds() : commonera.toAtomicSeconds();
	const ::com::diag::desperado::Epoch & epoch = platform.getEpoch();
	return commoneraseconds - epoch.seconds;
}

Epochalseconds Seconds::then(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int seconds) const {
	::com::diag::desperado::Platform & platform = ::com::diag::desperado::Platform::instance();
	::com::diag::desperado::CommonEra commonera(year, month, day, hour, minute, seconds);
	seconds_t commoneraseconds = platform.getLeapSecondTicks() ? commonera.toSeconds() : commonera.toAtomicSeconds();
	const ::com::diag::desperado::Epoch & epoch = platform.getEpoch();
	return (commoneraseconds > epoch.seconds) ? commoneraseconds - epoch.seconds : 0;
}

}
}
}
