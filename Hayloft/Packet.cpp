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
#include "com/diag/desperado/Dump.h"

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

void Packet::clear() {
	PacketData * here;
	while (head != 0) {
		here = head;
		head = here->next;
		delete here;
	}
	tail = 0;
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
	size_t appended;
	if (tail == 0) {
		append(*(new PacketBufferDynamic(allocation, PacketBuffer::EITHER)));
	} else if (tail->suffix() == 0) {
		append(*(new PacketBufferDynamic(allocation, PacketBuffer::APPEND)));
	}
	const PacketData::Datum * datap = static_cast<const PacketData::Datum*>(data);
	while (length > 0) {
		appended = tail->append(datap, length);
		if (appended > 0) {
			datap += appended;
			total += appended;
			length -= appended;
		}
		if (length > 0) {
			append(*(new PacketBufferDynamic(allocation, PacketBuffer::APPEND)));
		}
	}
	return total;
}

size_t Packet::prepend(const void * data, size_t length) {
	size_t total = 0;
	size_t prepended;
	size_t prefix;
	size_t actual;
	if (head == 0) {
		prepend(*(new PacketBufferDynamic(allocation, PacketBuffer::EITHER)));
	} else if (head->prefix() == 0) {
		prepend(*(new PacketBufferDynamic(allocation, PacketBuffer::PREPEND)));
	}
	// Complicated by the fact that we have to work backwards.
	const PacketData::Datum * datap = static_cast<const PacketData::Datum*>(data) + length;
	while (length > 0) {
		prefix = head->prefix();
		actual = (length > prefix) ? prefix : length;
		prepended = head->prepend(datap - actual, actual);
		// If prepended doesn't equal actual, we're screwed so badly it's not
		// really worth keeping separate variables.
		if (prepended > 0) {
			datap -= prepended;
			total += prepended;
			length -= prepended;
		}
		if (length > 0) {
			prepend(*(new PacketBufferDynamic(allocation, PacketBuffer::PREPEND)));
		}
	}
	return total;
}

size_t Packet::consume(void * data, size_t length) {
	size_t total = 0;
	size_t consumed;
	PacketData * here;
	PacketData::Datum * datap = static_cast<PacketData::Datum*>(data);
	while ((head != 0) && (length > 0)) {
		consumed = head->consume(datap, length);
		if (consumed > 0) {
			datap += consumed;
			total += consumed;
			length -= consumed;
		} else {
			here = head;
			head = here->next;
			delete here;
			if (head == 0) {
				tail = 0;
			}
		}
	}
	return total;
}

void Packet::show(int level, Output * display, int indent) const {
    ::com::diag::desperado::Platform& pl = ::com::diag::desperado::Platform::instance();
    ::com::diag::desperado::Print printf(display);
    ::com::diag::desperado::Dump dump(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    InputOutput::show(level, display, indent + 1);
    printf("%s allocation=%zu\n", sp, allocation);
    printf("%s head=%p\n", sp, head);
    if (0 < level) {
		for (PacketData * here = head; here != 0; here = here->next) {
			size_t length = here->length();
			size_t prefix = here->prefix();
			size_t size = here->size();
			size_t suffix = here->suffix();
			printf("%s  %p: next=%p length=%zu prefix=%zu size=%zu suffix=%zu total=%zu\n", sp, here, here->next, length, prefix, size, suffix, prefix + size + suffix);
			if (1 < level) {
			    dump.bytes(here->buffer(), size, false, 0, indent + 3);
			}
		}
    }
    printf("%s tail=%p\n", sp, tail);
   // in.show(level, display, indent + 1);
   // out.show(level, display, indent + 1);

}

}
}
}
