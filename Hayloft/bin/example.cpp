/* vim: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2012-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-hayloft<BR>R>
 */

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include "com/diag/grandote/PathInput.h"
#include "com/diag/grandote/PathOutput.h"
#include "com/diag/grandote/MaskableLogger.h"
#include "com/diag/grandote/size.h"
#include "com/diag/grandote/Thread.h"
#include "com/diag/hayloft/Credentials.h"
#include "com/diag/hayloft/Session.h"
#include "com/diag/hayloft/Simplex.h"
#include "com/diag/hayloft/Multiplex.h"
#include "com/diag/hayloft/Complex.h"
#include "com/diag/hayloft/BucketCreate.h"
#include "com/diag/hayloft/ObjectPut.h"
#include "com/diag/hayloft/ObjectCopy.h"
#include "com/diag/hayloft/ObjectGet.h"
#include "com/diag/hayloft/ObjectDelete.h"
#include "com/diag/hayloft/BucketDelete.h"

using namespace ::com::diag::hayloft;
using ::com::diag::grandote::PathInput;
using ::com::diag::grandote::PathOutput;
using ::com::diag::grandote::Thread;
using ::com::diag::grandote::MaskableLogger;

#define size(_ARGUMENT_)  ::com::diag::grandote::size(_ARGUMENT_)

static bool synchronicity() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("Synchronicity1");
		Bucket BUCKET2("Synchronicity2");
		Object OBJECT1("Synchronicity1.txt", BUCKET1);
		Object OBJECT2("Synchronicity2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());

		BucketCreate bucketcreate1(BUCKET1);
		BucketCreate bucketcreate2(BUCKET2);
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, input, length);
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2);
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, output);
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1);
		ObjectDelete objectdelete2(OBJECT2);
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1);
		BucketDelete bucketdelete2(BUCKET2);
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static bool simplexity() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("Simplexity1");
		Bucket BUCKET2("Simplexity2");
		Object OBJECT1("Simplexity1.txt", BUCKET1);
		Object OBJECT2("Simplexity2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());
		Simplex simplex;

		BucketCreate bucketcreate1(BUCKET1, simplex);
		BucketCreate bucketcreate2(BUCKET2, simplex);
		bucketcreate1.start();
		bucketcreate2.start();
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, simplex, input, length);
		objectput1.start();
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, simplex);
		objectcopy.start();
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, simplex, output);
		objectget2.start();
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, simplex);
		ObjectDelete objectdelete2(OBJECT2, simplex);
		objectdelete1.start();
		objectdelete2.start();
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, simplex);
		bucketdelete1.start();
		BucketDelete bucketdelete2(BUCKET2, simplex);
		bucketdelete2.start();
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static bool multiplexitycomplete() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("MultiplexityComplete1");
		Bucket BUCKET2("MultiplexityComplete2");
		Object OBJECT1("MultiplexityComplete1.txt", BUCKET1);
		Object OBJECT2("MultiplexityComplete2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());
		Multiplex multiplex;

		BucketCreate bucketcreate1(BUCKET1, multiplex);
		BucketCreate bucketcreate2(BUCKET2, multiplex);
		bucketcreate1.start();
		bucketcreate2.start();
		multiplex.complete();
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, multiplex, input, length);
		objectput1.start();
		multiplex.complete();
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
		objectcopy.start();
		multiplex.complete();
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, multiplex, output);
		objectget2.start();
		multiplex.complete();
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, multiplex);
		ObjectDelete objectdelete2(OBJECT2, multiplex);
		objectdelete1.start();
		objectdelete2.start();
		multiplex.complete();
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, multiplex);
		bucketdelete1.start();
		BucketDelete bucketdelete2(BUCKET2, multiplex);
		bucketdelete2.start();
		multiplex.complete();
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static bool multiplexityservice() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("MultiplexityService1");
		Bucket BUCKET2("MultiplexityService2");
		Object OBJECT1("MultiplexityService1.txt", BUCKET1);
		Object OBJECT2("MultiplexityService2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());
		Multiplex multiplex;

		BucketCreate bucketcreate1(BUCKET1, multiplex);
		BucketCreate bucketcreate2(BUCKET2, multiplex);
		bucketcreate1.start();
		bucketcreate2.start();
		while (multiplex.service() > 0) { }
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, multiplex, input, length);
		objectput1.start();
		while (multiplex.service() > 0) { }
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
		objectcopy.start();
		while (multiplex.service() > 0) { }
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, multiplex, output);
		objectget2.start();
		while (multiplex.service() > 0) { }
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, multiplex);
		ObjectDelete objectdelete2(OBJECT2, multiplex);
		objectdelete1.start();
		objectdelete2.start();
		while (multiplex.service() > 0) { }
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, multiplex);
		bucketdelete1.start();
		BucketDelete bucketdelete2(BUCKET2, multiplex);
		bucketdelete2.start();
		while (multiplex.service() > 0) { }
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static bool complexitypoll() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("ComplexityPoll1");
		Bucket BUCKET2("ComplexityPoll2");
		Object OBJECT1("ComplexityPoll1.txt", BUCKET1);
		Object OBJECT2("ComplexityPoll2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());
		Complex komplex;

		BucketCreate bucketcreate1(BUCKET1, komplex);
		komplex.start(bucketcreate1);
		BucketCreate bucketcreate2(BUCKET2, komplex);
		komplex.start(bucketcreate2);
		while (bucketcreate1 != true) { Thread::yield(); }
		while (bucketcreate2 != true) { Thread::yield(); }
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, komplex, input, length);
		komplex.start(objectput1);
		while (objectput1 != true) { Thread::yield(); }
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
		komplex.start(objectcopy);
		while (objectcopy != true) { Thread::yield(); }
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, komplex, output);
		komplex.start(objectget2);
		while (objectget2 != true) { Thread::yield(); }
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, komplex);
		komplex.start(objectdelete1);
		ObjectDelete objectdelete2(OBJECT2, komplex);
		komplex.start(objectdelete2);
		while (objectdelete1 != true) { Thread::yield(); }
		while (objectdelete2 != true) { Thread::yield(); }
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, komplex);
		komplex.start(bucketdelete1);
		BucketDelete bucketdelete2(BUCKET2, komplex);
		komplex.start(bucketdelete2);
		while (bucketdelete1 != true) { Thread::yield(); }
		while (bucketdelete2 != true) { Thread::yield(); }
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static bool complexitywait() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("ComplexityWait1");
		Bucket BUCKET2("ComplexityWait2");
		Object OBJECT1("ComplexityWait1.txt", BUCKET1);
		Object OBJECT2("ComplexityWait2.txt", BUCKET2);
		PathInput input(PATH);
		Octets length = size(PATH);
		PathOutput output(OBJECT2.getKey());
		Complex komplex;

		BucketCreate bucketcreate1(BUCKET1, komplex);
		komplex.start(bucketcreate1);
		BucketCreate bucketcreate2(BUCKET2, komplex);
		komplex.start(bucketcreate2);
		komplex.wait(bucketcreate1);
		komplex.wait(bucketcreate2);
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		ObjectPut objectput1(OBJECT1, komplex, input, length);
		komplex.start(objectput1);
		komplex.wait(objectput1);
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
		komplex.start(objectcopy);
		komplex.wait(objectcopy);
		if (!objectcopy.isSuccessful()) { break; }

		ObjectGet objectget2(OBJECT2, komplex, output);
		komplex.start(objectget2);
		komplex.wait(objectget2);
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, komplex);
		komplex.start(objectdelete1);
		ObjectDelete objectdelete2(OBJECT2, komplex);
		komplex.start(objectdelete2);
		komplex.wait(objectdelete1);
		komplex.wait(objectdelete2);
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, komplex);
		komplex.start(bucketdelete1);
		BucketDelete bucketdelete2(BUCKET2, komplex);
		komplex.start(bucketdelete2);
		komplex.wait(bucketdelete1);
		komplex.wait(bucketdelete2);
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

class MyObjectPut : public ObjectPut {
private:
	const char * path;
public:
	explicit MyObjectPut(const Object & object, const char * inputname)
	: ObjectPut(object, new PathInput(inputname), size(inputname))
	, path(inputname)
	{}
	explicit MyObjectPut(const Object & object, const Plex & plex, const char * inputname)
	: ObjectPut(object, plex, new PathInput(inputname), size(inputname))
	, path(inputname)
	{}
	virtual bool reset() {
		return ObjectPut::reset(new PathInput(path), size(path));
	}
};

class MyObjectGet : public ObjectGet {
private:
	const char * path;
public:
	explicit MyObjectGet(const Object & object, const char * outputname)
	: ObjectGet(object, new PathOutput(outputname))
	, path(outputname)
	{}
	explicit MyObjectGet(const Object & object, const Plex & plex, const char * outputname)
	: ObjectGet(object, plex, new PathOutput(outputname))
	, path(outputname)
	{}
	virtual bool reset() {
		return ObjectGet::reset(new PathOutput(path), 0, 0);
	}
};

static bool complexityrecoverable() {
	bool result = false;

	do {

		const char * PATH = "dat/unittest.txt";
		Bucket BUCKET1("ComplexityRecoverable1");
		Bucket BUCKET2("ComplexityRecoverable2");
		Object OBJECT1("ComplexityRecoverable1.txt", BUCKET1);
		Object OBJECT2("ComplexityRecoverable2.txt", BUCKET2);
		Complex komplex;

		BucketCreate bucketcreate1(BUCKET1, komplex);
		komplex.start(bucketcreate1);
		BucketCreate bucketcreate2(BUCKET2, komplex);
		komplex.start(bucketcreate2);
		komplex.wait(bucketcreate1);
		komplex.wait(bucketcreate2);
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, komplex, PATH);
		komplex.start(objectput1);
		komplex.wait(objectput1);
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, komplex);
		komplex.start(objectcopy);
		komplex.wait(objectcopy);
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, komplex, OBJECT2.getKey());
		komplex.start(objectget2);
		komplex.wait(objectget2);
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, komplex);
		komplex.start(objectdelete1);
		ObjectDelete objectdelete2(OBJECT2, komplex);
		komplex.start(objectdelete2);
		komplex.wait(objectdelete1);
		komplex.wait(objectdelete2);
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, komplex);
		komplex.start(bucketdelete1);
		BucketDelete bucketdelete2(BUCKET2, komplex);
		komplex.start(bucketdelete2);
		komplex.wait(bucketdelete1);
		komplex.wait(bucketdelete2);
		if (!bucketdelete1.isSuccessful()) { break; }
		if (!bucketdelete2.isSuccessful()) { break; }

		std::string command = "diff ";
		command += PATH;
		command += " ";
		command += OBJECT2.getKey();
		if (std::system(command.c_str()) < 0) { break; }
		if (::unlink(OBJECT2.getKey()) < 0) { break; }

		result = true;

	} while (false);

	return result;
}

static void usage(const char * program, FILE * stream)
{
    fprintf(stream, "\n");
    fprintf(stream, "usage: %s [ -b BUCKET_SUFFIX ] [ -i ACCESS_KEY_ID ] [ -k SECRET_ACCESS_KEY ] [ -u USER_AGENT ] [ -d ] [ -? ]\n", program);
    fprintf(stream, "       -b BUCKET_SUFFIX     Append BUCKET_SUFFIX to every canonical bucket name.\n");
    fprintf(stream, "       -d                   Enable debug output.\n");
    fprintf(stream, "       -h                   Display menu and exit.\n");
    fprintf(stream, "       -i ACCESS_KEY_ID     Use ACCESS_KEY_ID to authenticate with AWS.\n");
    fprintf(stream, "       -k SECRET_ACCESS_KEY Use SECRET_ACCESS_KEY to authenticate with AWS.\n");
    fprintf(stream, "       -u USER_AGENT        Specify USER_AGENT in every AWS operation.\n");
}

int main(int argc, char ** argv, char ** envp) {
    int xc = 0;

    const char * program = strrchr(argv[0], '/');
    program = (program == (char *)0) ? argv[0] : program + 1;

    const char * value;
    int opt;
    while ((opt = ::getopt(argc, argv, "b:dhi:k:u:")) >= 0) {
        switch (opt) {
        case 'b':
            ::setenv(Session::BUCKET_SUFFIX_ENV(), optarg, !0);
            break;
        case 'd':
            ::setenv(MaskableLogger::MASK_ENV(), "0xfff0", !0);
            MaskableLogger::instance().setMask();
            break;
        case 'h':
            usage(program, stdout);
            ::exit(0);
            break;
        case 'i':
			::setenv(Credentials::ACCESS_KEY_ID_ENV(), optarg, !0);
            break;
        case 'k':
			::setenv(Credentials::SECRET_ACCESS_KEY_ENV(), optarg, !0);
           break;
        case 'u':
			::setenv(Session::USER_AGENT_ENV(), optarg, !0);
            break;
        default:
            usage(program, stderr);
            ::exit(1);
            break;
        }
    }

    if (::getenv(Credentials::ACCESS_KEY_ID_ENV()) == (char *)0) {
    	fprintf(stderr, "Access key id UNDEFINED!\n");
        xc = 1;
    }

    if (::getenv(Credentials::SECRET_ACCESS_KEY_ENV()) == (char *)0) {
    	fprintf(stderr, "Secret access key UNDEFINED!\n");
        xc = 1;
    }

    if (xc != 0) {
        ::exit(xc);
    }

	fprintf(stderr, "Synchronous example starting.\n");
    if (synchronicity()) {
    	fprintf(stderr, "Synchronous example succeeded.\n");
    } else {
    	fprintf(stderr, "Synchronous example FAILED!\n");
    	xc = 2;
    }

	fprintf(stderr, "Simplex example starting.\n");
    if (simplexity()) {
    	fprintf(stderr, "Simplex example succeeded.\n");
    } else {
    	fprintf(stderr, "Simplex example FAILED!\n");
    	xc = 3;
    }

	fprintf(stderr, "Multiplex Complete example starting.\n");
    if (multiplexitycomplete()) {
    	fprintf(stderr, "Multiplex Complete example succeeded.\n");
    } else {
    	fprintf(stderr, "Multiplex Complete example FAILED!\n");
    	xc = 4;
    }

	fprintf(stderr, "Multiplex Service example starting.\n");
    if (multiplexityservice()) {
    	fprintf(stderr, "Multiplex Service example succeeded.\n");
    } else {
    	fprintf(stderr, "Multiplex Service example FAILED!\n");
    	xc = 5;
    }

	fprintf(stderr, "Complex Poll example starting.\n");
    if (complexitypoll()) {
    	fprintf(stderr, "Complex Poll example succeeded.\n");
    } else {
    	fprintf(stderr, "Complex Poll example FAILED!\n");
    	xc = 6;
    }

	fprintf(stderr, "Complex Wait example starting.\n");
    if (complexitywait()) {
    	fprintf(stderr, "Complex Wait example succeeded.\n");
    } else {
    	fprintf(stderr, "Complex Wait example FAILED!\n");
    	xc = 6;
    }

	fprintf(stderr, "Complex Recoverable example starting.\n");
    if (complexityrecoverable()) {
    	fprintf(stderr, "Complex Recoverable example succeeded.\n");
    } else {
    	fprintf(stderr, "Complex Recoverable example FAILED!\n");
    	xc = 7;
    }

    ::exit(xc);
}
