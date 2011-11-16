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
class BufferOutput;
class DataInput;
class DescriptorInput;
class DescriptorOutput;
class FileInput;
class FileOutput;
class Input;
class Output;
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

Size size(const ::com::diag::desperado::BufferOutput & output);

Size size(const ::com::diag::desperado::DataInput & input);

Size size(const ::com::diag::desperado::DescriptorInput & input);

Size size(const ::com::diag::desperado::DescriptorOutput & output);

Size size(const ::com::diag::desperado::FileInput & input);

Size size(const ::com::diag::desperado::FileOutput & output);

Size size(const ::com::diag::desperado::Input & input);

Size size(const ::com::diag::desperado::Output & output);

Size size(const PacketInput & input);

Size size(const Packet & packet);

Size size(const PacketData & data);

}
}
}

#endif
