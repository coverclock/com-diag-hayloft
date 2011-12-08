/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_FIBBONACCI_
#define _H_COM_DIAG_HAYLOFT_FIBBONACCI_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

namespace com {
namespace diag {
namespace hayloft {

/**
 * Fibbonacci implements the Fibbonacci sequence for use in back off during
 * error recovery and retry. There is no provision for handling integer
 * overflow.
 */
class Fibbonacci {

private:

	unsigned int was;

	unsigned int now;

public:

	/**
	 * Ctor.
	 */
	Fibbonacci()
	: was(0)
	, now(1)
	{}

	/**
	 * Return the current number in the sequence and advance the sequence.
	 *
	 * @return the current number in the sequence.
	 */
	unsigned int next() {
		int result = now + was;
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
