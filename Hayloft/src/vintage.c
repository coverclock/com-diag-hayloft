/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * The purpose of this translation unit is to embed the vintage string
 * inside the library or shared object. The object module will be statically
 * linked into an application only if the translation unit makes explicit
 * references to the storage here as external references.
 */

#include "com/diag/hayloft/vintage.h"

const char COM_DIAG_HAYLOFT_VINTAGE_KEYWORD[] = "COM_DIAG_HAYLOFT_VINTAGE=" COM_DIAG_HAYLOFT_VINTAGE;
const char * COM_DIAG_HAYLOFT_VINTAGE_VALUE = &COM_DIAG_HAYLOFT_VINTAGE_KEYWORD[sizeof("COM_DIAG_HAYLOFT_VINTAGE=") - 1];
