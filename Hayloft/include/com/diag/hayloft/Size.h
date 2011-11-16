/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_SIZE_
#define _H_COM_DIAG_HAYLOFT_SIZE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "com/diag/desperado/target.h"
#include "com/diag/desperado/stdio.h"

namespace com {
namespace diag {
namespace desperado {
class BufferInput;
class DataInput;
class DescriptorInput;
class FileInput;
class Input;
}
}
}

namespace com {
namespace diag {
namespace hayloft {

class Packet;
class PacketData;
class PacketInput;

typedef off_t Size;

Size size(const char * path);

Size size(int fd);

Size size(const FILE * fp);

Size size(const ::com::diag::desperado::BufferInput & input);

Size size(const ::com::diag::desperado::DataInput & input);

Size size(const ::com::diag::desperado::DescriptorInput & input);

Size size(const ::com::diag::desperado::FileInput & input);

Size size(const ::com::diag::desperado::Input & input);

Size size(const PacketInput & input);

Size size(const Packet & packet);

Size size(const PacketData & data);

}
}
}

#endif
