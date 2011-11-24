/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAYLOFT_README_
#define _H_COM_DIAG_HAYLOFT_README_

/*******************************************************************************

	Copyright 2011 by the Digital Aggregates Corporation, Colorado, USA.

--------------------------------------------------------------------------------

	LICENSE

	This file is part of the Digital Aggregates Corporation Hayloft package.

    Hayloft is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hayloft is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Hayloft.  If not, see <http://www.gnu.org/licenses/>.

	Alternative commercial licensing terms are available from the copyright
	holder.

		Digital Aggregates Corporation
		3440 Youngfield Street, #209
		Wheat Ridge CO 80033 USA
		http://www.diag.com
		mailto:jsloan@diag.com

--------------------------------------------------------------------------------

	ABSTRACT

	This file is part of the Digital Aggregates Corporation Hayloft package,
	which, among other things, presents a C++-based abstraction around Bryan
	Ischo's excellent C-based libs3 interface to Amazon Web Service (AWS)
	Simple Storage Service (S3). It is built on top of the Digital Aggregates
	Corporation Desperadito package, a C++ systems programming library.

	This software is an original work of its author.

	Information regarding Hayloft can be found here:

		http://www.diag.com/navigation/downloads/Hayloft

	Information regarding libs3 can be found here:

		http://aws.amazon.com/developertools/1648
		http://libs3.ischo.com/index.html
		http://libs3.ischo.com.s3.amazonaws.com/dox/index.html

	Information regarding Desperado can be found here:

		http://www.diag.com/navigation/downloads/Desperadito
		http://www.diag.com/navigation/downloads/Desperado

	Contact:

		Chip Overclock
		mailto:coverclock@diag.com
		Digital Aggregates Corporation
		3440 Youngfield Street, #209
		Wheat Ridge CO 80033 USA
		http://www.diag.com

--------------------------------------------------------------------------------

	INSTALLATION

	Versions of Hayloft have been built and tested on the following platforms:

		Dell Inspiron 530 i686
		Ubuntu 10.04 "Lucid Lynx"
		Linux 2.6.32
		GCC 4.4.3
		libs3 2.0
		Desperadito 6.4.0 (Desperado "Empire")
		AWS S3 API 2006-03-01

	See the Makefile for more information.

--------------------------------------------------------------------------------

	REFERENCES

	Amazon.com, Amazon Simple Storage Service, Getting Started Guide (GSG),
	API Version 2006-03-01, Amazon Web Services

	Amazon.com, Amazon Simple Storage Service, Developer Guide (DG),
	API Version 2006-03-01, Amazon Web Services

	Amazon.com, Amazon Simple Storage Service, API Reference (AR),
	API Version 2006-03-01, Amazon Web Services

	Amazon.com, Amazon Simple Storage Service, Console User Guide,
	API Version 2006-03-01, Amazon Web Services

	B. Ischo, libs3.h, http://libs3.ischo.com.s3.amazonaws.com/index.html

	B. Ischo, s3.c, http://libs3.ischo.com.s3.amazonaws.com/index.html

	J. Sloan, Hayloft, http://www.diag.com/navigation/downloads/Hayloft

	J. Sloan, Desperado, http://www.diag.com/navigation/downloads/Desperado

--------------------------------------------------------------------------------

	ENVIRONMENTAL VARIABLES

	COM_DIAG_HAYLOFT_S3_CREDENTIALS_ACCESSKEYID

		This string is your twenty character AWS access key id, a sort of
		login (but not your AWS web login).

	COM_DIAG_HAYLOFT_S3_CREDENTIALS_SECRETACCESSKEY

		This string is your forty character AWS secret access key, a sort of
		password (but not your AWS web password) and encryption key.

	COM_DIAG_HAYLOFT_S3_ENDPOINT_HOSTNAME

		This string is the DNS host name of the AWS S3 service. This host name
		may (or must) differ depending on the region the bucket is in. If not
		specified, the United States Standard host name, "s3.amazonaws.com",
		is used.

	COM_DIAG_HAYLOFT_S3_SESSION_BUCKETSUFFIX

		If specified, this string is is appended without further punctuation
		to each bucket name specified in any of the Bucket* constructors
		(BucketCreate, etc.). Bucket names have to be globally unique. I
		recommend using your DNS domain name with a leading dot, for example
		".foo.bar.com".

	COM_DIAG_HAYLOFT_S3_SESSION_USERAGENT

		If specified, this string is passed to S3 as the User Agent meta-data
		to identify your application.

	COM_DIAG_HAYLOFT_LOGGER_MASK

		If specified, this string contains a sixteen bit number, encoded using
		the normal C syntax rules for octal, decimal, or hexadecimal numbers,
		to specify a sixteen bit logging mask at run time. The bits in the mask
		which of the sixteen log levels are enabled via the expression
		(1 << LEVEL) where LEVEL is one of the following log levels. So for
		example the DEBUG level is enabled if bit (1 << DEBUG) or 0x10 is set.
		The different log levels are mapped to appropriate log levels in the
		underlying logging mechanism; for example, SYSLOG has eight log levels.
		If not specified, the default logger mask is used, for example 0xff80.

		FINEST          =   0,
        FINER           =   1,
        FINE            =   2,
        TRACE           =   3,
        DEBUG           =   4,
        INFORMATION     =   5,
        CONFIGURATION   =   6,
        NOTICE          =   7,
        WARNING         =   8,
        ERROR           =   9,
        SEVERE          =   10,
        CRITICAL        =   11,
        ALERT           =   12,
        FATAL           =   13,
        EMERGENCY       =   14,
        PRINT           =   15

*******************************************************************************/

/**
 *  @file
 *
 *  This is the README for this project.
 */

#endif
