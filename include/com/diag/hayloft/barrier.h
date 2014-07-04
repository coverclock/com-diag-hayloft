/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_MEMORY_BARRIER_
#define _H_COM_DIAG_HAYLOFT_MEMORY_BARRIER_

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

#if !defined(com_diag_hayloft_memory_barrier)
#	if defined(__GNUC__)
#		if defined(__GNUC_MINOR__)
#			if ((((__GNUC__)*1000)+(__GNUC_MINOR__))>=4001)
#				define com_diag_hayloft_memory_barrier() __sync_synchronize()
#			endif
# 		endif
#	endif
#endif

/*
 * Implementations for other targets and platforms can be defined here.
 */

#if !defined(com_diag_hayloft_memory_barrier)
#	define com_diag_hayloft_memory_barrier() ((void)0)
#	warning No memory barrier!
#endif

/**
 * Implements a memory barrier using the native mechanisms if such
 * is available on the target and platform. Otherwise degenerates to doing
 * nothing. A built-in memory fence was implemented in GCC starting
 * in 4.1.
 *
 * This is basically the same implementation as the Desperado barrier but
 * falls back to doing nothing rather than trying to make do with some
 * POSIX threads side effects.
 *
 * Since I haven't figured out how to adequately unit test this,
 * this must be considered highly experimental.
 */
inline void memory_barrier(void) {
	com_diag_hayloft_memory_barrier();
}

}
}
}

#endif
