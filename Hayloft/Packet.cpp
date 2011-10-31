/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <new>
#include <cstdio>
#include <cstring>
#include <cerrno>
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
		const Datum * here = static_cast<const Datum*>(data);
		memcpy(pointer, here + length - actual, actual);
		head = pointer;
	}
	return actual;
}

size_t PacketData::consume(void * buffer, size_t length) {
	size_t available = tail - head;
	size_t actual = (available > length) ? length : available;
	if (actual > 0) {
		if (buffer != 0) {
			memcpy(buffer, head, actual);
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
		append(*(new PacketBufferDynamic(allocation, fraction)));
	} else if (tail->suffix() == 0) {
		append(*(new PacketBufferDynamic(allocation, PacketBufferDynamic::APPEND)));
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
			append(*(new PacketBufferDynamic(allocation, PacketBufferDynamic::APPEND)));
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
		prepend(*(new PacketBufferDynamic(allocation, fraction)));
	} else if (head->prefix() <= 0) {
		prepend(*(new PacketBufferDynamic(allocation, PacketBufferDynamic::PREPEND)));
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
			prepend(*(new PacketBufferDynamic(allocation, PacketBufferDynamic::PREPEND)));
		}
	}
	return total;
}

size_t Packet::consume(void * buffer, size_t length) {
	size_t total = 0;
	size_t consumed;
	PacketData * here;
	PacketData::Datum * bufferp = static_cast<PacketData::Datum*>(buffer);
	while ((head != 0) && (length > 0)) {
		consumed = head->consume(bufferp, length);
		if (consumed > 0) {
			if (bufferp != 0) { bufferp += consumed; }
			total += consumed;
			length -= consumed;
		}
		if ((consumed <= 0) || head->empty()) {
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

size_t Packet::source(::com::diag::desperado::Input& from) {
	size_t total = 0;
	size_t subtotal;
	ssize_t length;
	PacketData::Datum * data;
	PacketDataDynamic * pbd;
	do {
		data = new PacketData::Datum [allocation];
		subtotal = 0;
		do {
			length = from(data + subtotal, 1, allocation - subtotal);
			if (length <= 0) {
				break;
			}
			subtotal += length;
		} while (subtotal < allocation);
		if (subtotal > 0) {
			pbd = new PacketDataDynamic(data, subtotal, PacketDataDynamic::APPEND);
			append(*pbd);
			total += subtotal;
		} else {
			delete data;
		}
	} while (length > 0);
	return total;
}

size_t Packet::sink(::com::diag::desperado::Output& to) {
	size_t total = 0;
	size_t subtotal;
	ssize_t length;
	size_t size;
	const PacketData::Datum * data;
	PacketData * here;
	while (head != 0) {
		data = static_cast<const PacketData::Datum*>(head->buffer());
		size = head->size();
		subtotal = 0;
		while (size > subtotal) {
			length = to(data + subtotal, 1, size - subtotal);
			if (length <= 0) {
				break;
			}
			subtotal += length;
		}
		total += subtotal;
		here = head;
		head = here->next;
		delete here;
		if (head == 0) {
			tail = 0;
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
    com::diag::desperado::InputOutput::show(level, display, indent + 1);
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
    printf("%s in:\n", sp);
	in.show(level, display, indent + 2);
    printf("%s out:\n", sp);
	out.show(level, display, indent + 2);
}

/*******************************************************************************
 * PacketInput
 ******************************************************************************/

int PacketInput::operator() () {
	char ch;
	if (packet.consume(&ch, sizeof(ch)) > 0) {
		return ch;
	} else {
		errno = 0;
		return EOF;
	}
}

int PacketInput::operator() (int c) {
	char ch = c;
	packet.prepend(&ch, sizeof(ch));
	return ch;
}

ssize_t PacketInput::operator() (char * buffer, size_t size) {
	ssize_t total = 0;
	if (buffer == 0) {
		// Do nothing.
	} else if (size <= 0) {
		// Do nothing.
	} else {
		size_t length;
		while (size > 1) {
			length = packet.consume(buffer, sizeof(*buffer));
			if (length > 0) {
				size -= length;
				total += length;
				buffer += length;
				if ((*(buffer - 1) == '\n') || (*(buffer - 1) == '\0')) {
					break;
				}
			} else {
				if (total <= 0) {
					total = EOF;
					errno = 0;
				}
				break;
			}
		}
		if ((total != EOF) && (*(buffer - 1) != '\0')) {
			*buffer = '\0';
			++total;
		}
	}
	return total;
}

ssize_t PacketInput::operator() (void * buffer, size_t /* minimum */, size_t maximum) {
	size_t length = packet.consume(buffer, maximum);
	if (length > 0) {
		return length;
	} else {
		errno = 0;
		return EOF;
	}
}

void PacketInput::show(int level, com::diag::desperado::Output * display, int indent) const {
    ::com::diag::desperado::Platform& pl = ::com::diag::desperado::Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    com::diag::desperado::Input::show(level, display, indent + 1);
    printf("%s packet=%p\n", sp, &packet);
}

/*******************************************************************************
 * PacketOutput
 ******************************************************************************/

int PacketOutput::operator() (int c) {
	char ch = c;
	packet.append(&ch, sizeof(ch));
	return ch;
}

ssize_t PacketOutput::operator() (const char * s, size_t size) {
	size_t length = ::strnlen(s, size);
	return packet.append(s, length);
}

ssize_t PacketOutput::operator() (const char * format, va_list ap) {
	char buffer[::com::diag::desperado::Output::minimum_buffer_size + 1];
    ::vsnprintf(buffer, sizeof(buffer), format, ap);
    size_t length = ::strnlen(buffer, sizeof(buffer));
    return packet.append(buffer, length);
}

ssize_t PacketOutput::operator() (const void * buffer, size_t /* minimum */, size_t maximum) {
	return packet.append(buffer, maximum);
}

int PacketOutput::operator() () {
	return 0;
}

void PacketOutput::show(int level, com::diag::desperado::Output * display, int indent) const {
	::com::diag::desperado::Platform& pl = ::com::diag::desperado::Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    com::diag::desperado::Output::show(level, display, indent + 1);
    printf("%s packet=%p\n", sp, &packet);
}

}
}
}
