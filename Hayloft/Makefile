################################################################################
# Copyright 2011-2012 by the Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in README.h
# Chip Overclock <coverclock@diag.com>
# http://www.diag.com/navigation/downloads/Hayloft
################################################################################

PROJECT=hayloft
MAJOR=2
MINOR=6
BUILD=0

SVN_URL=svn://graphite/$(PROJECT)/trunk/Hayloft
HTTP_URL=http://www.diag.com/navigation/downloads/Hayloft.html

TMP=/tmp

################################################################################
# APPLICATION PREREQUISITES
################################################################################

# DESPERADO is a C++ systems programming library that includes a platform and
# I/O abstraction layer that I've found extremely useful. HAYLOFT works fine
# with the full blown DESPERADO, but it is quite large. I recommend using
# DESPERADITO, which is a subset of DESPERADO with a reduced footprint.

DESPERADO_DIR=$(HOME)/src/Desperadito#http://www.diag.com/navigation/downloads/Desperadito.html
DESPERADO_LIBS=$(DESPERADO_DIR)
DESPERADO_INC=$(DESPERADO_DIR)/include#/desperado

# This is Bryan Ischo's excellent C-based LIBS3 package. I built it using the
# following command and then access the build artifacts directly from its
# build directories:
# make deb

S3_DIR=$(HOME)/src/libs3-2.0#http://libs3.ischo.com.s3.amazonaws.com/libs3-2.0.tar.gz
S3_LIBS=$(S3_DIR)/build/deb/usr/lib
S3_INC=$(S3_DIR)/inc

# LIBS3 uses OPENSSL, CURL, and XML2. I installed these using my server's
# standard package manager.

OPENSSL_DIR=#Downloaded via Synaptic Package Manager
OPENSSL_LIBS=/lib
OPENSSL_INC=/usr/include#/openssl

CURL_DIR=#Downloaded via Synaptic Package Manager
CURL_LIBS=/usr/lib
CURL_INC=/usr/include#/curl

XML2_DIR=#Downloaded via Synaptic Package Manager
XML2_LIBS=/usr/lib
XML2_INC=/usr/include#/libxml2

# This is set up for dynamic linking. Remember that you will need to either
# explicitly specify the LD_LIBRARY_PATH environmental variable or (preferred)
# use the ldconfig utility to tell the system about the shared objects it
# doesn't already know about (for this application, typically DESPERADITO and
# LIBS3).
# To display the shared objects used by the unittest executable:
# ldd unittest
# To configure the DESPERADITO and LIBS3 shared objects for use (adjust paths):
# sudo ldconfig ${HOME}/src/Desperadito ${HOME}/src/libs3-2.0/build/deb/usr/lib

APPLICATION_CPPFLAGS=-I$(DESPERADO_INC) -I$(S3_INC)
APPLICATION_LDFLAGS=-L$(DESPERADO_LIBS) -ldesperadito -L$(S3_LIBS) -ls3 -L$(LIBXML2_LIBS) -lxml2 -L$(CURL_LIBS) -lcurl -L$(OPENSSL_LIBS) -lssl -lcrypto

################################################################################
# UNIT TEST PREREQUISITES
################################################################################

# I installed GMOCK which includes GTEST, but so far haven't used the GMOCK
# functionality in this particular project. So feel free just to download GTEST
# and modify the directory path appropriately both here and in LARIAT that sits
# on top of it.

GMOCK_DIR=$(HOME)/src/gmock-1.6.0#http://googlemock.googlecode.com/files/gmock-1.6.0.zip
GMOCK_LIB=$(GMOCK_DIR)/lib/.libs/libgmock.a
GMOCK_INC=$(GMOCK_DIR)/include

GTEST_DIR=$(GMOCK_DIR)/gtest
GTEST_LIB=$(GTEST_DIR)/lib/.libs/libgtest.a
GTEST_INC=$(GTEST_DIR)/include

#GTEST_DIR=$(HOME)/src/gtest-1.6.0#http://googletest.googlecode.com/files/gtest-1.6.0.zip
#GTEST_LIB=$(GTEST_DIR)/libgtest.a
#GTEST_INC=$(GTEST_DIR)/include

ifdef GMOCK_LIB
GTEST_INCS=-I$(GMOCK_INC) -I$(GTEST_INC)
GTEST_LIBS=$(GMOCK_LIB) $(GTEST_LIB)
else
GTEST_INCS=-I$(GTEST_INC)
GTEST_LIBS=$(GTEST_LIB)
endif

# LARIAT is a thin wrapper around GTEST that allows you to place resource
# constraints (in particular, time) around unit tests. This prevents a single
# unit test from going into a loop or deadlocking and hanging up the entire
# test suite for a large code base. It can be built with either just GTEST or
# with GMOCK which includes GTEST.

LARIAT_DIR=$(HOME)/src/Lariat#http://www.diag.com/navigation/downloads/Lariat.html
LARIAT_LIB=$(LARIAT_DIR)/liblariat.a
LARIAT_INC=$(LARIAT_DIR)/include#/lariat

# Google recommends static linking GTEST (and GMOCK), so that is what we do
# with both it and LARIAT.

UNITTEST_CPPFLAGS=-I$(LARIAT_INC) $(GTEST_INCS)
UNITTEST_LDFLAGS=$(LARIAT_LIB) $(GTEST_LIBS)

################################################################################
# PROJECT
################################################################################

PROJECT_DIR:=$(shell pwd)

HAYLOFT_DIR=$(PROJECT_DIR)
HAYLOFT_LIBS=$(HAYLOFT_DIR)
HAYLOFT_LIB=lib$(PROJECT).so
HAYLOFT_INC=include

PROJECT_CPPFLAGS=-I$(HAYLOFT_INC)
PROJECT_LDFLAGS=-L$(HAYLOFT_LIBS) -lhayloft
#PROJECT_LDFLAGS=lib$(PROJECT).a# For testing static linking.

CC=$(CROSS_COMPILE)gcc
CXX=$(CROSS_COMPILE)g++
AR=$(CROSS_COMPILE)ar
RANLIB=$(CROSS_COMPILE)ranlib
STRIP=$(CROSS_COMPILE)strip

CPPFLAGS=$(PROJECT_CPPFLAGS) $(APPLICATION_CPPFLAGS) $(UNITTEST_CPPFLAGS)
CFLAGS=-g
CXXFLAGS=-g
ARFLAGS=rcv
SOFLAGS=xv
LDFLAGS=-g $(PROJECT_LDFLAGS) $(APPLICATION_LDFLAGS) $(UNITTEST_LDFLAGS) -lpthread -lrt -lm

################################################################################
# LISTS
################################################################################

PHONY=# Make these targets every time regardless of dependencies.

TARGETS=# Make these targets for make all.

ARTIFACTS=# Remove these targets for make clean.

DELIVERABLES=# Remove these targets for make clobber.

ARCHIVABLE=# Include these targets in the archive or shared library.

################################################################################
# DEFAULT
################################################################################

PHONY+=default

default:	all

################################################################################
# MANIFESTS
################################################################################

MANIFEST_O=\
 hayloft/Condition.o \
 hayloft/Logger.o \
 hayloft/Packet.o \
 hayloft/Parameter.o \
 hayloft/Seconds.o \
 hayloft/size.o \
 hayloft/Thread.o \
 hayloft/Uncancellable.o \
 s3/Access.o \
 s3/Action.o \
 s3/Bucket.o \
 s3/BucketCreate.o \
 s3/BucketDelete.o \
 s3/BucketHead.o \
 s3/BucketManifest.o \
 s3/Complex.o \
 s3/Conditions.o \
 s3/Container.o \
 s3/Context.o \
 s3/Credentials.o \
 s3/Endpoint.o \
 s3/Grant.o \
 s3/GrantGet.o \
 s3/GrantSet.o \
 s3/LifeCycle.o \
 s3/Log.o \
 s3/LogGet.o \
 s3/LogSet.o \
 s3/Multiplex.o \
 s3/Object.o \
 s3/ObjectCopy.o \
 s3/ObjectDelete.o \
 s3/ObjectGet.o \
 s3/ObjectHead.o \
 s3/ObjectPut.o \
 s3/Plex.o \
 s3/Properties.o \
 s3/Protocol.o \
 s3/Region.o \
 s3/Selection.o \
 s3/Service.o \
 s3/ServiceManifest.o \
 s3/Session.o \
 s3/Style.o \
 s3/convergence.o \
 s3/show.o \
 s3/tostring.o

################################################################################
# BUILD
################################################################################

TARGETS+=$(MANIFEST_O)
ARTIFACTS+=$(MANIFEST_O)
ARCHIVABLE+=$(MANIFEST_O)

################################################################################
# LIBRARIES AND SHARED OBJECTS
################################################################################

TARGETS+=lib$(PROJECT).a
DELIVERABLES+=lib$(PROJECT).a

lib$(PROJECT).a:	$(ARCHIVABLE)
	$(AR) $(ARFLAGS) lib$(PROJECT).a $(ARCHIVABLE)
	$(RANLIB) lib$(PROJECT).a

TARGETS+=lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR).$(MINOR) lib$(PROJECT).so.$(MAJOR) lib$(PROJECT).so
DELIVERABLES+=lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR).$(MINOR) lib$(PROJECT).so.$(MAJOR) lib$(PROJECT).so

lib$(PROJECT).so lib$(PROJECT).so.$(MAJOR) lib$(PROJECT).so.$(MAJOR).$(MINOR) lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD):	lib$(PROJECT).a
	HERE="`pwd`"; \
	THERE="`mktemp -d $(TMP)/$(PROJECT).XXXXXXXXXX`"; \
	( cd $$THERE; $(AR) $(SOFLAGS) $$HERE/lib$(PROJECT).a ); \
	$(CC) $(CARCH) -shared -Wl,-soname,lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) -o lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) $$THERE/*.o; \
	rm -rf $$THERE
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR).$(MINOR)
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so.$(MAJOR)
	ln -s -f lib$(PROJECT).so.$(MAJOR).$(MINOR).$(BUILD) lib$(PROJECT).so

PHONY+=LD_LIBRARY_PATH

LD_LIBRARY_PATH:
	@echo "export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS)"

################################################################################
# UNIT TESTS
################################################################################

TARGETS+=unittest
ARTIFACTS+=unittest.o
DELIVERABLES+=unittest

COM_DIAG_HAYLOFT_LIBS3_CURL_PROXY=127.0.0.1:8888
COM_DIAG_HAYLOFT_LOGGER_MASK=0xfff0
COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE=1
COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT=512
COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME=60

unittest:	unittest.o $(HAYLOFT_LIB) $(LARIAT_LIB) $(GMOCK_LIB) $(GTEST_LIB) $(DESPERADO_LIB) $(S3_LIB) $(XML2_LIB) $(CURL_LIB) $(OPENSSL_LIB) $(CRYPTO_LIB)
	$(CXX) -o unittest unittest.o $(LDFLAGS)

PHONY+=run

run:	unittest
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	time ./unittest

PHONY+=debug

debug:	unittest
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE="$(COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	time ./unittest

PHONY+=capture

capture:	unittest
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "time ./unittest --gtest_color=no" capture.log

PHONY+=test

test:	unittest
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE="$(COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "time ./unittest --gtest_color=no --gtest_filter=$(FILTER)" test.log # e.g. FILTER=BucketFilter.Explicit
		
PHONY+=suite

suite:	unittest
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE="$(COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "time ./unittest --gtest_color=no" suite.log
	
PHONY+=proxy

proxy:	unittest
	export COM_DIAG_HAYLOFT_LIBS3_CURL_PROXY="$(COM_DIAG_HAYLOFT_LIBS3_CURL_PROXY)"; \
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE="$(COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "./unittest --gtest_color=no" proxy.log
	
PHONY+=memcheck

memcheck:	unittest
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "valgrind --tool=memcheck ./unittest --gtest_color=no" memcheck.log
	
PHONY+=helgrind

helgrind:	unittest
	export COM_DIAG_HAYLOFT_LOGGER_MASK="$(COM_DIAG_HAYLOFT_LOGGER_MASK)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_LIMIT)"; \
	export COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME="$(COM_DIAG_HAYLOFT_LIBS3_CURL_LOW_SPEED_TIME)"; \
	export LD_LIBRARY_PATH=$(HAYLOFT_LIBS):$(DESPERADO_LIBS):$(S3_LIBS); \
	script -c "valgrind --tool=helgrind ./unittest --gtest_color=no" helgrind.log

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

# This is a lot more difficult than it ought to be.

DEPENDS:=${shell find . -type f \( -name '*.c' -o -name '*.cpp' \) -print}

depend:
	cp /dev/null dependencies.mk
	for F in $(DEPENDS); do \
		D=`dirname $$F | sed "s/^\.\///"`; \
		echo -n "$$D/" >> dependencies.mk; \
		$(CXX) $(CPPFLAGS) -MM -MG $$F >> dependencies.mk; \
	done

-include dependencies.mk

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
# DOCUMENTATION
################################################################################

DOC_DIR=doc
BROWSER=firefox

PHONY+=documentation browse refman manpages
DELIVERABLES+=$(DOC_DIR)

documentation:
	mkdir -p $(DOC_DIR)/latex $(DOC_DIR)/man $(DOC_DIR)/pdf
	sed -e "s/\\\$$Name.*\\\$$/$(MAJOR).$(MINOR).$(BUILD)/" < doxygen.cf > doxygen-local.cf
	doxygen doxygen-local.cf
	( cd $(DOC_DIR)/latex; $(MAKE) refman.pdf; cp refman.pdf ../pdf )
	cat $(DOC_DIR)/man/man3/*.3 | groff -man -Tps - > $(DOC_DIR)/pdf/manpages.ps
	ps2pdf $(DOC_DIR)/pdf/manpages.ps $(DOC_DIR)/pdf/manpages.pdf

browse:
	$(BROWSER) file:doc/html/index.html

refman:
	$(BROWSER) file:doc/pdf/refman.pdf

manpages:
	$(BROWSER) file:doc/pdf/manpages.pdf

################################################################################
# PATCHES
################################################################################

PHONY+=libs3-2.0.patch

libs3-2.0.patch:
	diff -purN $(HOME)/src/libs3-2.0-org/src/request.c $(S3_DIR)/src/request.c > libs3-2.0.patch || true

PHONY+=patch

patch:
	( cd $(S3_DIR); patch -p5 ) < libs3-2.0.patch

################################################################################
# ENTRY POINTS
################################################################################

PHONY+=all clean clobber pristine

all:	$(TARGETS)

clean:
	rm -rf $(ARTIFACTS)
	
clobber:	clean
	rm -rf $(DELIVERABLES)

################################################################################
# END
################################################################################

.PHONY:	$(PHONY)
