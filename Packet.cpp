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
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"

namespace com {
namespace diag {
namespace hayloft {

/*******************************************************************************
 * PacketData
 ******************************************************************************/

size_t PacketData::append(const void * data, size_t length) {
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

size_t PacketData::prepend(const void * data, size_t length) {
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

/*******************************************************************************
 * Packet
 ******************************************************************************/

Packet::~Packet() {
	PacketData * here;
	while (head != 0) {
		here = head;
		head = here->next;
		delete here;
	}
}

void Packet::append(PacketData & rd) {
	rd.next = 0;
	if (tail == 0) {
		head = &rd;
		tail = &rd;
	} else {
		tail->next = &rd;
		tail = &rd;
	}
}

void Packet::prepend(PacketData & rd) {
	rd.next = head;
	if (head == 0) {
		head = &rd;
		tail = &rd;
	} else {
		head = &rd;
	}
}

size_t Packet::append(const void * data, size_t length) {
	size_t total = 0;
	size_t produced;
	if (tail == 0) {
		append(*(new PacketBuffer(allocation, 2)));
	} else if (tail->suffix() == 0) {
		append(*(new PacketBuffer(allocation, unsignedintmaxof(size_t))));
	}
	while (length > 0) {
		produced = tail->append(data, length);
		if (produced > 0) {
			data += produced;
			total += produced;
			length -= produced;
		}
		if (produced < length) {
			append(*(new PacketBuffer(allocation, unsignedintmaxof(size_t))));
		}
	}
	return total;
}

size_t Packet::prepend(const void * data, size_t length) {
	size_t total = 0;
	size_t produced;
	if (head == 0) {
		prepend(*(new PacketBuffer(allocation, 2)));
	} else if (head->prefix() == 0) {
		prepend(*(new PacketBuffer(allocation, 1)));
	}
	while (length > 0) {

		if (produced < length) {
			prepend(*(new PacketBuffer(allocation, 1)));
		}
	}
	return total;
}

size_t Packet::consume(void * data, size_t length) {
	size_t total = 0;
	size_t consumed;
	PacketData * here;
	while ((head != 0) && (length > 0)) {
		consumed = head->consume(data, length);
		if (consumed > 0) {
			data += consumed;
			total += consumed;
			length -= consumed;
		} else {
			here = head;
			head = here->next;
			delete here;
		}
	}
	return total;
}

void Packet::show(int level, Output * display, int indent) const {
    ::com::diag::desperado::Platform& pl = ::com::diag::desperado::Platform::instance();
    ::com::diag::desperado::Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    InputOutput::show(level, display, indent + 1);
    printf("%s allocation=%zu\n", sp, allocation);
    printf("%s head=%lu\n", sp, head);
    printf("%s tail=%lu\n", sp, tail);
   // in.show(level, display, indent + 1);
   // out.show(level, display, indent + 1);

}

}
}
}
