com-diag-hayloft
================

Copyright 2011-2014 by the Digital Aggregates Corporation, Colorado, USA.

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

Documentation for Hayloft is licensed under the Creative Commons
Attribution-Noncommercial-Share Alike 3.0 United States License.
See <http://creativecommons.org/licenses/by-nc-sa/3.0/us/>.

Alternative commercial licensing terms are available from the copyright
holder.

ABSTRACT

This file is part of the Digital Aggregates Corporation Hayloft package.
Hayloft presents a C++-based object oriented abstraction around Bryan
Ischo's excellent C-based libs3 interface to Amazon Web Service (AWS) Simple
Storage Service (S3). It is built on top of the Digital Aggregates
Corporation Desperadito package, a C++ systems programming library that is
a fork from the Corporation's Desperado package. libs3 is built on top of
the cURL, Open SSL, and XML2 libraries, which I acquired through the normal
package manager on my server. I've also made patches to libs3 to support the
use of the Charles Web Debugging Proxy application by Karl von Randow and
cURL verbosity.

This software is an original work of its author.

Information regarding Hayloft can be found here:

    http://www.diag.com/navigation/downloads/Hayloft

Information regarding libs3 can be found here:

    http://aws.amazon.com/developertools/1648
    http://libs3.ischo.com/index.html
    http://libs3.ischo.com.s3.amazonaws.com/dox/index.html

Information regarding Desperadito and Desperado can be found here:

    http://www.diag.com/navigation/downloads/Desperadito
    http://www.diag.com/navigation/downloads/Desperado

Information regarding the Charles Web Debugging Proxy can be found here:

    http://www.charlesproxy.com/

Contact:

    Chip Overclock
    Digital Aggregates Corporation
    3440 Youngfield Street, #209
    Wheat Ridge CO 80033 USA
    http://www.diag.com
    mailto:coverclock@diag.com

--------------------------------------------------------------------------------

INSTALLATION

Hayloft have been built and tested on the following platform(s):

    Dell Inspiron 530 i686
    Ubuntu 10.04 "Lucid Lynx"
    Linux 2.6.32
    GCC 4.4.3
    libs3 2.0
    Desperadito 7.0.0 (Desperado "Inspiration")
    AWS S3 API 2006-03-01

See the Makefile for more information.

--------------------------------------------------------------------------------

REFERENCES

C. Overclock, "Amazon.com's Simple Storage Service and Hayloft",
http://coverclock.blogspot.com/2011/12/amazons-simple-storage-service-and.html

C. Overclock, "Eventual Consistency and Hayloft",
http://coverclock.blogspot.com/2011/12/eventual-consistency-and-hayloft.html

C. Overclock, "Abstraction in C++ using I/O Functors",
http://coverclock.blogspot.com/2011/11/abstraction-in-c-using-io-functors.html

Amazon.com, Amazon Simple Storage Service, "Getting Started Guide (GSG)",
API Version 2006-03-01, Amazon Web Services

Amazon.com, Amazon Simple Storage Service, "Developer Guide (DG)",
API Version 2006-03-01, Amazon Web Services

Amazon.com, Amazon Simple Storage Service, "API Reference (AR)",
API Version 2006-03-01, Amazon Web Services

Amazon.com, Amazon Simple Storage Service, "Console User Guide",
API Version 2006-03-01, Amazon Web Services

Amazon Simple Storage Service Team, "Best Practices for Using Amazon S3",
2008-11-10, Amazon Web Services

Jinesh Varia, "Cloud Architectures", 2008-06, Amazon Web Services

Jinesh Varia, "Architecting for the Cloud: Best Practices", 2010-01,
Amazon Web Services

R. Fielding et al., "Hypertext Transfer Protocol -- HTTP/1.1", RFC2616,
The Internet Society, June 1999

B. Ischo, libs3.h, http://libs3.ischo.com.s3.amazonaws.com/index.html

B. Ischo, s3.c, http://libs3.ischo.com.s3.amazonaws.com/index.html

J. Sloan, Hayloft, http://www.diag.com/navigation/downloads/Hayloft

J. Sloan, Desperadito, http://www.diag.com/navigation/downloads/Desperadito

--------------------------------------------------------------------------------

ENVIRONMENTAL VARIABLES

    COM_DIAG_HAYLOFT_S3_CREDENTIALS_ACCESSKEYID

This string is your twenty character AWS access key id,
a sort of login (but not your AWS web login).

    COM_DIAG_HAYLOFT_S3_CREDENTIALS_SECRETACCESSKEY

This string is your forty character AWS secret access
key, a sort of password (but not your AWS web password)
and encryption key.

    COM_DIAG_HAYLOFT_S3_ENDPOINT_HOSTNAME

This string is the DNS host name of the AWS S3
service. This host name may (or must) differ depending on
the region the bucket is in. If not specified, the United
States Standard end point, "s3.amazonaws.com", is used.

    COM_DIAG_HAYLOFT_S3_SESSION_BUCKETSUFFIX

If specified, this string is is appended without further
punctuation to each bucket name specified in any of the
Bucket constructors (BucketCreate, etc.) prior to the
bucket name being canonicalized by Hayloft. Canonicalized
bucket names have to be globally unique and form a legal
internet Domain Name Service (DNS) domain name. Although
domain names are case insensitive, S3 requires that
they be all lower case. I recommend using your DNS
domain name with a leading dot as a bucket suffix,
for example ".foo.bar.com". An application creating a
bucket using BucketCreate("Me") and using the default end
point name and the default virtual path style URL will
hence see the name canonicalized into an S3 bucket name
"me.foo.bar.com.s3.amazonaws.com".  Canonicalized bucket
names are what you will see in a ServiceManifest.

    COM_DIAG_HAYLOFT_S3_SESSION_USERAGENT

If specified, this string is passed to S3 in the
User-Agent HTTP header to identify your application.

    COM_DIAG_HAYLOFT_S3_REGION_NAME

This string is the default AWS region to use. If not
specified, the United States Standard region is used
(which is actually specified by omitting the region
parameter altogether in messages to S3).

    COM_DIAG_HAYLOFT_LOGGER_MASK

If specified, this string contains a sixteen bit number,
encoded using the normal C syntax rules for octal,
decimal, or hexadecimal numbers, to specify a sixteen bit
logging mask at run time. The bits in the mask which of
the sixteen log levels are enabled via the expression
(1 << LEVEL) where LEVEL is one of the following log
levels. So for example the DEBUG level is enabled if bit
(1 << DEBUG) or 0x0010 is set.	The different log levels
found in com/diag/desperado/Log.h are the transitive
closure of common logging mechanisms from GNU/Linux,
Apache, Java, etc.

    ENUMERATION		VALUE	MASK

    FINEST           0		0x0001
    FINER            1		0x0002
    FINE             2		0x0004
    TRACE            3		0x0008
    DEBUG            4		0x0010
    INFORMATION      5		0x0020
    CONFIGURATION    6		0x0040
    NOTICE           7		0x0080
    WARNING          8		0x0100
    ERROR            9		0x0200
    SEVERE          10		0x0400
    CRITICAL        11		0x0800
    ALERT           12		0x1000
    FATAL           13		0x2000
    EMERGENCY       14		0x4000
    PRINT           15		0x8000

    MASK			LOGGING

    0xff80			errors, warnings, notices of unusual events (default)
    0xffc0			plus convergence and retry events like timeouts
    0xffe0			plus output from show functions
    0xfff0			plus debug output

The Desperado log levels are mapped to the appropriate log levels in the
underlying logging mechanism. For example, SYSLOG has eight log levels.


    COM_DIAG_HAYLOFT_LIBS3_CURL_PROXY

This string specifies the IP address and port used
for the cURL PROXY option used in conjunction with
the Charles Web Debugging Proxy application. Typically
this will be the local host address and Charles port
"127.0.0.1:8888". This capability is only available if
your libs3 sources have been patched with the patch file
in the Hayloft distribution, and is only necessary if
you are using Charles to spy on the communication between
your system and S3. This might be useful for debugging.

libcurl and Charles don't seem to play well together when
switching back and forth between HTTP and HTTPS. HTTPS
is the default. Running the entire unit test suite
with proxying enabled seems to cause Charles to break
the BucketTest.Explicit unit test, which is the only
unit test that uses HTTP. Running that unit test all by
itself works fine. Running all unit tests without Charles
works fine. As far as I can tell, libcurl is not reusing
an existing HTTPS socket connection when switching to
HTTP for BucketTest.Explicit, which I thought might be
the problem.

    COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE

This string, if it is equal to "1", turns on the cURL
VERBOSE option.  This causes CURL, the URL library used
by libs3, to log just about everything it does. This
might be useful for debugging.

    COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT
    COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME

If S3 does not maintain a bandwidth of at least the
LOW_SPEED_LIMIT in bytes per second during a window
of LOW_SPEED_TIME seconds, libcurl will terminate the
connection. libs3 sets these parameters to 1024 and
15 respectively. These strings, when converted to long
values, can be used to change this limit. Typically you'll
want to set them lower. I find that even with a good
broadband internet connection using a pretty sizeable
Pentium server I sometimes see only around 660B/s. I
suspect this is my broadband provider throttling my
uplink speed. This will be an issue during ObjectGet
and ObjectPut Actions.

--------------------------------------------------------------------------------

PLEXES

Plexes are different approaches to managing the execution of
Actions. You can see examples of all of them in the unit test
suite. Briefly here are the different Plexes you can use.

None

When you use a constructor that doesn't specify a Plex,
you are using the synchronous interface. The Action
will be started inside the Action constructor and the
calling thread will block until the Action completes.
It is up to you to decide whether you need to restart
the Action for any reason.

Simplex

The Simplex uses the synchronous interface but doesn't
start the Action automatically. You must start it manually
using its start method. The calling thread will block in
the start method until the Action completes.  It is up
to you to decide whether you need to restart the Action
for any reason.

Multiplex

The Multiplex uses the asynchronous interface. You must
start the Action manually using the start method, and
then use the methods provided by the Multiplex to drive
the underlying state machines inside libs3 and libcurl
to talk to S3. Typically you will run these Multiplex
methods in the foreground thread. It is possible to run
them in a background thread, but you are responsible for
any synchronization. It is up to you to decide whether
you need to restart the Action for any reason.

Complex

Complex uses the asynchronous interface. You don't
start the Action at all, but instead submit it to the
Complex background thread for starting.  You can call
the wait method which will block your thread until a
specific Action has completed. Complex automatically
retries any Actions that complete due to a recoverable
error, or for errors which are likely to be caused by
eventual consistency issues. Complex can run many Actions
simultaneously. Once an Action completes, either it has
completed for a reason that is non-retryable (including
success), it has already been retried too many times,
or it failed to start or to reset upon a restart.
