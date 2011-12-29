/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_FIBONACCI_
#define _H_COM_DIAG_HAYLOFT_FIBONACCI_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * Fibonacci implements the Fibonacci sequence for use in back off during
 * error recovery and retry. The resulting Fibonacci number is limited to
 * no more than the the maximum possible value of the underlying integer
 * data type.
 */
class Fibonacci {

private:

	unsigned int was;

	unsigned int now;

public:

	/**
	 * Ctor.
	 */
	Fibonacci()
	: was(0)
	, now(1)
	{}

	/**
	 * Return the current number in the sequence and advance the sequence.
	 *
	 * @return the current number in the sequence.
	 */
	unsigned int next() {
		int result = (now <= (intmaxof(unsigned int) - was)) ? now + was : now;
		was = now;
		now = result;
		return result;
	}

	/**
	 * Return the current number in the sequence and advance the sequence.
	 *
	 * @return the current number in the sequence.
	 */
	operator unsigned int() {
		return next();
	}

	/**
	 * Return the current number in the sequence without advancing the sequence.
	 *
	 * @return the current number in the sequence.
	 */
	unsigned int get() {
		return now;
	}

	/**
	 * Reset the sequence back to the beginning.
	 */
	void reset() {
		was = 0;
		now = 1;
	}

};

}
}
}

#endif
