/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/stat.h>
#include <unistd.h>
#include "com/diag/hayloft/Size.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/hayloft/Packet.h"

namespace com {
namespace diag {
namespace hayloft {

Size size(const char * path) {
	Size result = EOF;
	struct ::stat status;
	if (path == 0) {
		// Do nothing: invalid!
	} else if (::stat(path, &status) < 0) {
		// Do nothing: failed!
	} else if (!S_ISREG(status.st_mode)) {
		// Do nothing: cannot determine!
	} else {
		result = status.st_size;
	}
	return result;
}

Size size(int fd) {
	Size result = EOF;
	struct ::stat status;
	if (fd < 0) {
		// Do nothing: invalid!
	} else if (::fstat(fd, &status) < 0) {
		// Do nothing: failed!
	} else if (!S_ISREG(status.st_mode)) {
		// Do nothing: cannot determine!
	} else {
		result = status.st_size;
	}
	return result;
}

Size size(const FILE * fp) {
	Size result = EOF;
	if (fp == 0) {
		// Do nothing: invalid!
	} else {
		// Remarkably, fileno(3) does not declare its argument to be const.
		// I can't believe it actually alters the FILE structure in any
		// meaningful way. So I'm totally cheating here.
		result = size(::fileno(const_cast<FILE *>(fp)));
	}
	return result;
}

Size size(const ::com::diag::desperado::BufferInput & input) {
	return input.getLength();
}

Size size(const ::com::diag::desperado::BufferOutput & output) {
	return output.getLength();
}

Size size(const ::com::diag::desperado::DataInput & input) {
	return input.getLength();
}

Size size(const ::com::diag::desperado::DescriptorInput & input) {
	return (size(input.getDescriptor()) + input.getPushed());
}

Size size(const ::com::diag::desperado::DescriptorOutput & output) {
	return size(output.getDescriptor());
}

Size size(const ::com::diag::desperado::FileInput & input) {
	return size(input.getFile());
}

Size size(const ::com::diag::desperado::FileOutput & output) {
	return size(output.getFile());
}

Size size(const ::com::diag::desperado::Input & input) {
	return EOF;
}

Size size(const ::com::diag::desperado::Output & output) {
	return EOF;
}

Size size(const PacketInput & input) {
	return input.getLength();
}

Size size(const Packet & packet) {
	return packet.length();
}

Size size(const PacketData & data) {
	return data.length();
}

}
}
}
