################################################################################
# Copyright 2011 by the Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in README.h
# Chip Overclock <coverclock@diag.com>
# http://www.diag.com/navigation/downloads/Hayloft
################################################################################

PROJECT=hayloft
MAJOR=0
MINOR=8
BUILD=0

SVN_URL=svn://graphite/$(PROJECT)/trunk/Hayloft
HTTP_URL=http://www.diag.com/navigation/downloads/Hayloft.html

TMP=/tmp

################################################################################
# LISTS
################################################################################

PHONY=# Make these targets every time regardless of dependencies.

TARGETS=# Make these targets for make all.

ARTIFACTS=# Remove these targets for make clean.

DELIVERABLES=# Remove these targets for make clobber.

ARCHIVABLE=# Include these targets in the archive or shared library.

################################################################################
# APPLICATION PREREQUISITES
################################################################################

OPENSSL_DIR=#Synaptic Package Manager
OPENSSL_LIBS=/lib
OPENSSL_LIB=$(OPENSSL_LIBS)/libssl.so.0.9.8
CRYPTO_LIB=$(OPENSSL_LIBS)/libcrypto.so.0.9.8
OPENSSL_INC=/usr/include#/openssl

CURL_DIR=#Synaptic Package Manager
CURL_LIBS=/usr/lib
CURL_LIB=$(CURL_LIBS)/libcurl.so
CURL_INC=/usr/include#/curl

XML2_DIR=#Synaptic Package Manager
XML2_LIBS=/usr/lib
XML2_LIB=$(XML2_LIBS)/libxml2.so
XML2_INC=/usr/include#/libxml2

S3_DIR=$(HOME)/src/libs3-2.0#http://libs3.ischo.com.s3.amazonaws.com/libs3-2.0.tar.gz
S3_LIBS=$(S3_DIR)/build/deb/usr/lib
S3_LIB=$(S3_LIBS)/libs3.so
S3_INC=$(S3_DIR)/inc

DESPERADO_DIR=$(HOME)/src/Desperadito#http://www.diag.com/ftp/desperadito-6.4.0.tgz
DESPERADO_LIBS=$(DESPERADO_DIR)
DESPERADO_LIB=$(DESPERADO_LIBS)/libdesperadito.so
DESPERADO_INC=$(DESPERADO_DIR)/include#/desperado

APPLICATION_CPPFLAGS=-I$(DESPERADO_INC) -I$(S3_INC)
APPLICATION_LDFLAGS=-L$(DESPERADO_LIBS) -ldesperadito -L$(S3_LIBS) -ls3 -L$(LIBXML2_LIBS) -lxml2 -L$(CURL_LIBS) -lcurl -L$(OPENSSL_LIBS) -lssl -lcrypto

################################################################################
# UNIT TEST PREREQUISITES
################################################################################

TINYXML_DIR=$(HOME)/src/tinyxml#http://sourceforge.net/projects/tinyxml/
TINYXML_LIBS=$(TINYXML_DIR)
TINYXML_LIB=$(TINYXML_LIBS)/libtinyxml.a
TINYXML_INC=$(TINYXML_DIR)

LARIAT_DIR=$(HOME)/src/Lariat#http://www.diag.com/ftp/lariat-0.1.0.tgz
LARIAT_LIBS=$(LARIAT_DIR)
LARIAT_LIB=$(LARIAT_LIBS)/liblariat.a
LARIAT_INC=$(LARIAT_DIR)/include

GMOCK_DIR=$(HOME)/src/gmock-1.6.0#http://googlemock.googlecode.com/files/gmock-1.6.0.zip
GMOCK_LIBS=$(GMOCK_DIR)/lib/.libs
GMOCK_LIB=$(GMOCK_LIBS)/libgmock.a
GMOCK_INC=$(GMOCK_DIR)/include

#GTEST_DIR=$(HOME)/src/gtest-1.6.0#http://googletest.googlecode.com/files/gtest-1.6.0.zip
#GTEST_LIBS=$(GTEST_DIR)
#GTEST_LIB=$(GTEST_LIBS)/libgtest.a
#GTEST_INC=$(GTEST_DIR)/include

GTEST_DIR=$(GMOCK_DIR)/gtest#http://googlemock.googlecode.com/files/gmock-1.6.0.zip
GTEST_LIBS=$(GTEST_DIR)/lib/.libs
GTEST_LIB=$(GTEST_LIBS)/libgtest.a
GTEST_INC=$(GTEST_DIR)/include

UNITTEST_CPPFLAGS=-I$(LARIAT_INC) -I$(GMOCK_INC) -I$(GTEST_INC)
UNITTEST_LDFLAGS=$(LARIAT_LIB) $(GMOCK_LIB) $(GTEST_LIB)

################################################################################
# PROJECT
################################################################################

CWD:=$(shell pwd)
PROJECT_DIR=$(CWD)

HAYLOFT_DIR=.
HAYLOFT_LIBS=.
HAYLOFT_LIB=lib$(PROJECT).so
HAYLOFT_INC=include

PROJECT_CPPFLAGS=-I$(HAYLOFT_INC)
PROJECT_LDFLAGS=-L$(HAYLOFT_LIBS) -lhayloft

CC=gcc
CXX=g++
AR=ar
RANLIB=ranlib
STRIP=strip

CPPFLAGS=$(PROJECT_CPPFLAGS) $(APPLICATION_CPPFLAGS) $(UNITTEST_CPPFLAGS)
CFLAGS=-g
CXXFLAGS=-g
ARFLAGS=rcv
SOFLAGS=xv
LDFLAGS=-g $(PROJECT_LDFLAGS) $(APPLICATION_LDFLAGS) $(UNITTEST_LDFLAGS) -lpthread -lrt -lm

################################################################################
# DEFAULT
################################################################################

PHONY+=default

default:	all

################################################################################
# BUILD
################################################################################

TARGETS+=hayloft/Logger.o
ARTIFACTS+=hayloft/Logger.o
ARCHIVABLE+=hayloft/Logger.o

TARGETS+=hayloft/Packet.o
ARTIFACTS+=hayloft/Packet.o
ARCHIVABLE+=hayloft/Packet.o

TARGETS+=hayloft/Parameter.o
ARTIFACTS+=hayloft/Parameter.o
ARCHIVABLE+=hayloft/Parameter.o

TARGETS+=hayloft/Size.o
ARTIFACTS+=hayloft/Size.o
ARCHIVABLE+=hayloft/Size.o

TARGETS+=s3/Session.o
ARTIFACTS+=s3/Session.o
ARCHIVABLE+=s3/Session.o

TARGETS+=s3/Credentials.o
ARTIFACTS+=s3/Credentials.o
ARCHIVABLE+=s3/Credentials.o

TARGETS+=s3/Region.o
ARTIFACTS+=s3/Region.o
ARCHIVABLE+=s3/Region.o

TARGETS+=s3/Protocol.o
ARTIFACTS+=s3/Protocol.o
ARCHIVABLE+=s3/Protocol.o

TARGETS+=s3/Style.o
ARTIFACTS+=s3/Style.o
ARCHIVABLE+=s3/Style.o

TARGETS+=s3/Access.o
ARTIFACTS+=s3/Access.o
ARCHIVABLE+=s3/Access.o

TARGETS+=s3/Context.o
ARTIFACTS+=s3/Context.o
ARCHIVABLE+=s3/Context.o

TARGETS+=s3/Multiplex.o
ARTIFACTS+=s3/Multiplex.o
ARCHIVABLE+=s3/Multiplex.o

TARGETS+=s3/Show.o
ARTIFACTS+=s3/Show.o
ARCHIVABLE+=s3/Show.o

TARGETS+=s3/Properties.o
ARTIFACTS+=s3/Properties.o
ARCHIVABLE+=s3/Properties.o

TARGETS+=s3/Conditions.o
ARTIFACTS+=s3/Conditions.o
ARCHIVABLE+=s3/Conditions.o

TARGETS+=s3/Bucket.o
ARTIFACTS+=s3/Bucket.o
ARCHIVABLE+=s3/Bucket.o

TARGETS+=s3/BucketValid.o
ARTIFACTS+=s3/BucketValid.o
ARCHIVABLE+=s3/BucketValid.o

TARGETS+=s3/BucketTest.o
ARTIFACTS+=s3/BucketTest.o
ARCHIVABLE+=s3/BucketTest.o

TARGETS+=s3/BucketCreate.o
ARTIFACTS+=s3/BucketCreate.o
ARCHIVABLE+=s3/BucketCreate.o

TARGETS+=s3/BucketDelete.o
ARTIFACTS+=s3/BucketDelete.o
ARCHIVABLE+=s3/BucketDelete.o

TARGETS+=s3/Object.o
ARTIFACTS+=s3/Object.o
ARCHIVABLE+=s3/Object.o

TARGETS+=s3/ObjectPut.o
ARTIFACTS+=s3/ObjectPut.o
ARCHIVABLE+=s3/ObjectPut.o

TARGETS+=s3/ObjectGet.o
ARTIFACTS+=s3/ObjectGet.o
ARCHIVABLE+=s3/ObjectGet.o

TARGETS+=s3/ObjectDelete.o
ARTIFACTS+=s3/ObjectDelete.o
ARCHIVABLE+=s3/ObjectDelete.o

TARGETS+=s3/ObjectHead.o
ARTIFACTS+=s3/ObjectHead.o
ARCHIVABLE+=s3/ObjectHead.o

TARGETS+=unittest
ARTIFACTS+=unittest

unittest:	unittest.o $(HAYLOFT_LIB) $(LARIAT_LIB) $(GMOCK_LIB) $(GTEST_LIB) $(DESPERADO_LIB) $(S3_LIB) $(XML2_LIB) $(CURL_LIB) $(OPENSSL_LIB) $(CRYPTO_LIB)
	$(CXX) -o unittest unittest.o $(LDFLAGS)

################################################################################
# LIBRARIES AND SHARED OBJECTS
################################################################################

DELIVERABLES+=lib$(PROJECT).a

lib$(PROJECT).a:	$(ARCHIVABLE)
	$(AR) $(ARFLAGS) lib$(PROJECT).a $(ARCHIVABLE)
	$(RANLIB) lib$(PROJECT).a

DELIVERABLES+=lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD)

lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD):	lib$(PROJECT).a
	HERE="`pwd`"; \
	THERE="`mktemp -d $(TMP)/$(PROJECT).XXXXXXXXXX`"; \
	( cd $$THERE; $(AR) $(SOFLAGS) $$HERE/lib$(PROJECT).a ); \
	$(CC) $(CARCH) -shared -Wl,-soname,lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) -o lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) $$THERE/*.o; \
	rm -rf $$THERE

DELIVERABLES+=lib$(PROJECT).so.$(MAJOR).$(MINOR)

lib$(PROJECT).so.$(MAJOR).$(MINOR):	lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD)
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR).$(MINOR)

DELIVERABLES+=lib$(PROJECT).so.$(MAJOR)

lib$(PROJECT).so.$(MAJOR):	lib$(PROJECT).so.$(MAJOR).$(MINOR)
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR)

DELIVERABLES+=lib$(PROJECT).so

lib$(PROJECT).so:	lib$(PROJECT).so.$(MAJOR)
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so

################################################################################
# UNIT TESTS
################################################################################

PHONY+=test

test:	unittest
	./unittest
	echo "PASSED all"

################################################################################
# PATTERNS
################################################################################

%.txt:	%.cpp
	$(CXX) -E $(CPPFLAGS) -c $< > $*.txt

%.o:	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

%.txt:	%.c
	$(CC) -E $(CPPFLAGS) -c $< > $*.txt

%.o:	%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

%:	%_unstripped
	$(STRIP) -o $@ $<

################################################################################
# DEPENCENDIES
################################################################################

PHONY+=depend

ARTIFACTS+=dependencies.mk

depend:
	DEPENDS="`find . -type f \( -name '*.c' -o -name '*.cpp' \) -print`"; \
	echo "DEPENDS=\"$$DEPENDS\""; \
	$(CXX) $(CPPFLAGS) -M -MG $$DEPENDS > dependencies.mk

-include dependencies.mk

################################################################################
# ENTRY POINTS
################################################################################

PHONY+=all clean clobber pristine

all:	$(TARGETS)

clean:
	rm -f $(ARTIFACTS)
	
clobber:	clean
	rm -f $(DELIVERABLES)

################################################################################
# DISTRIBUTION
################################################################################

PHONY+=dist

ARTIFACTS+=$(PROJECT)-$(MAJOR).$(MINOR).$(BUILD).tgz

dist $(PROJECT)-$(MAJOR).$(MINOR).$(BUILD).tgz:
	TARDIR=$(shell mktemp -d /tmp/$(PROJECT).XXXXXXXXXX); \
	svn export $(SVN_URL) $$TARDIR/$(PROJECT)-$(MAJOR).$(MINOR).$(BUILD); \
	tar -C $$TARDIR -cvzf - $(PROJECT)-$(MAJOR).$(MINOR).$(BUILD) > $(PROJECT)-$(MAJOR).$(MINOR).$(BUILD).tgz; \
	rm -rf $$TARDIR

################################################################################
# END
################################################################################

.PHONY:	$(PHONY)
