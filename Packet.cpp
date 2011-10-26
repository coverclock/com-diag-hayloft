/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <new>
#include "com/diag/hayloft/Packet.h"

namespace com {
namespace diag {
namespace hayloft {

size_t PacketData::append(void * data, size_t length) {
	size_t available = (tail == 0) ? extent : payload + extent - tail;
	size_t actual = (available > length) ? length : available;
	if (actual > 0) {
		Datum * pointer;
		if (tail == 0) {
			size_t slack = (available - actual) / fraction;
			pointer = payload + slack;
			head = pointer;
		} else {
			pointer = tail;
		}
		memcpy(pointer, data, actual);
		tail = pointer + actual;
	}
	return actual;
}

size_t PacketData::prepend(void * data, size_t length) {
	size_t available = (head == 0) ? extent : head - payload;
	size_t actual = (available > length) ? length : available;
	if (actual > 0) {
		Datum * pointer;
		if (head == 0) {
			size_t slack = (available - actual) / fraction;
			pointer = payload + slack;
			tail = pointer + actual;
		} else {
			pointer = head - actual;
		}
		memcpy(pointer, data, actual);
		head = pointer;
	}
	return actual;
}

size_t PacketData::consume(void * data, size_t length) {
	size_t available = tail - head;
	size_t actual = (available > length) ? length : available;
	if (actual > 0) {
		if (data != 0) {
			memcpy(data, head, actual);
		}
		if (actual < available) {
			head += actual;
		} else {
			head = 0;
			tail = 0;
		}
	}
	return actual;
}

}
}
}
