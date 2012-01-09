/**
 * @file
 *
 * Copyright 2012 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/hayloft/Logger.h"
#include "com/diag/hayloft/size.h"
#include "com/diag/hayloft/s3/Credentials.h"
#include "com/diag/hayloft/s3/Session.h"
#include "com/diag/hayloft/s3/Simplex.h"
#include "com/diag/hayloft/s3/Multiplex.h"
#include "com/diag/hayloft/s3/Complex.h"
#include "com/diag/hayloft/s3/BucketCreate.h"
#include "com/diag/hayloft/s3/ObjectPut.h"
#include "com/diag/hayloft/s3/ObjectCopy.h"
#include "com/diag/hayloft/s3/ObjectGet.h"
#include "com/diag/hayloft/s3/ObjectDelete.h"
#include "com/diag/hayloft/s3/BucketDelete.h"

using namespace ::com::diag::hayloft;
using namespace ::com::diag::hayloft::s3;
using ::com::diag::desperado::PathInput;
using ::com::diag::desperado::PathOutput;

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
	virtual bool reset(bool force = false) {
		return ObjectPut::reset(new PathInput(path), size(path), force);
	}
};

class MyObjectGet : public ::com::diag::hayloft::s3::ObjectGet {
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
	virtual bool reset(bool force = false) {
		return ObjectGet::reset(new PathOutput(path), 0, 0, force);
	}
};

static bool synchronicity() {
	bool result = false;

	do {

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("Synchronicity1");
		Bucket BUCKET2("Synchronicity2");
		Object OBJECT1("Synchronicity1.txt", BUCKET1);
		Object OBJECT2("Synchronicity2.txt", BUCKET2);

		BucketCreate bucketcreate1(BUCKET1);
		BucketCreate bucketcreate2(BUCKET2);
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, PATH);
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2);
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, OBJECT2.getKey());
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

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("Simplexity1");
		Bucket BUCKET2("Simplexity2");
		Object OBJECT1("Simplexity1.txt", BUCKET1);
		Object OBJECT2("Simplexity2.txt", BUCKET2);
		Simplex simplex;

		BucketCreate bucketcreate1(BUCKET1, simplex);
		BucketCreate bucketcreate2(BUCKET2, simplex);
		bucketcreate1.start();
		bucketcreate2.start();
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, simplex, PATH);
		objectput1.start();
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, simplex);
		objectcopy.start();
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, simplex, OBJECT2.getKey());
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

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("MultiplexityComplete1");
		Bucket BUCKET2("MultiplexityComplete2");
		Object OBJECT1("MultiplexityComplete1.txt", BUCKET1);
		Object OBJECT2("MultiplexityComplete2.txt", BUCKET2);
		Multiplex multiplex;

		BucketCreate bucketcreate1(BUCKET1, multiplex);
		BucketCreate bucketcreate2(BUCKET2, multiplex);
		bucketcreate1.start();
		bucketcreate2.start();
		multiplex.complete();
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, multiplex, PATH);
		objectput1.start();
		multiplex.complete();
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
		objectcopy.start();
		multiplex.complete();
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, multiplex, OBJECT2.getKey());
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

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("MultiplexityService1");
		Bucket BUCKET2("MultiplexityService2");
		Object OBJECT1("MultiplexityService1.txt", BUCKET1);
		Object OBJECT2("MultiplexityService2.txt", BUCKET2);
		Multiplex multiplex;

		BucketCreate bucketcreate1(BUCKET1, multiplex);
		BucketCreate bucketcreate2(BUCKET2, multiplex);
		bucketcreate1.start();
		bucketcreate2.start();
		while (multiplex.service() > 0) { }
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, multiplex, PATH);
		objectput1.start();
		while (multiplex.service() > 0) { }
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, multiplex);
		objectcopy.start();
		while (multiplex.service() > 0) { }
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, multiplex, OBJECT2.getKey());
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

static bool complexity() {
	bool result = false;

	do {

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("Complexity1");
		Bucket BUCKET2("Complexity2");
		Object OBJECT1("Complexity1.txt", BUCKET1);
		Object OBJECT2("Complexity2.txt", BUCKET2);
		Complex complex;

		BucketCreate bucketcreate1(BUCKET1, complex);
		complex.start(bucketcreate1);
		BucketCreate bucketcreate2(BUCKET2, complex);
		complex.start(bucketcreate2);
		complex.wait(bucketcreate1);
		complex.wait(bucketcreate2);
		if (!bucketcreate1.isSuccessful()) { break; }
		if (!bucketcreate2.isSuccessful()) { break; }

		MyObjectPut objectput1(OBJECT1, complex, PATH);
		complex.start(objectput1);
		complex.wait(objectput1);
		if (!objectput1.isSuccessful()) { break; }

		ObjectCopy objectcopy(OBJECT1, OBJECT2, complex);
		complex.start(objectcopy);
		complex.wait(objectcopy);
		if (!objectcopy.isSuccessful()) { break; }

		MyObjectGet objectget2(OBJECT2, complex, OBJECT2.getKey());
		complex.start(objectget2);
		complex.wait(objectget2);
		if (!objectget2.isSuccessful()) { break; }

		ObjectDelete objectdelete1(OBJECT1, complex);
		complex.start(objectdelete1);
		ObjectDelete objectdelete2(OBJECT2, complex);
		complex.start(objectdelete2);
		complex.wait(objectdelete1);
		complex.wait(objectdelete2);
		if (!objectdelete1.isSuccessful()) { break; }
		if (!objectdelete2.isSuccessful()) { break; }

		BucketDelete bucketdelete1(BUCKET1, complex);
		complex.start(bucketdelete1);
		BucketDelete bucketdelete2(BUCKET2, complex);
		complex.start(bucketdelete2);
		complex.wait(bucketdelete1);
		complex.wait(bucketdelete2);
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
    fprintf(stream, "usage: %s [ -b BUCKET_SUFFIX ] [ -i ACCESS_KEY_ID ] [ -k SECRET_ACCESS_KEY ] [ -u USER_AGENT ] [ -! ] [ -? ]\n", program);
    fprintf(stream, "       -b BUCKET_SUFFIX     Append BUCKET_SUFFIX to every canonical bucket name.\n");
    fprintf(stream, "       -i ACCESS_KEY_ID     Use ACCESS_KEY_ID to authenticate with AWS.\n");
    fprintf(stream, "       -k SECRET_ACCESS_KEY Use SECRET_ACCESS_KEY to authenticate with AWS.\n");
    fprintf(stream, "       -u USER_AGENT        Specify USER_AGENT in every AWS operation.\n");
    fprintf(stream, "       -!                   Enable debug output.\n");
    fprintf(stream, "       -?                   Display menu.\n");
}

int main(int argc, char ** argv, char ** envp) {
	int xc = 0;

    const char * program = strrchr(argv[0], '/');
    program = (program == (char *)0) ? argv[0] : program + 1;

	const char * value;
    int opt;
    while ((opt = ::getopt(argc, argv, "b:i:k:u:!?")) >= 0) {
        switch (opt) {
        case 'b':
			::setenv(::com::diag::hayloft::s3::Session::BUCKET_SUFFIX_ENV(), optarg, !0);
            break;
        case 'i':
			::setenv(::com::diag::hayloft::s3::Credentials::ACCESS_KEY_ID_ENV(), optarg, !0);
            break;
        case 'k':
			::setenv(::com::diag::hayloft::s3::Credentials::SECRET_ACCESS_KEY_ENV(), optarg, !0);
           break;
        case 'u':
			::setenv(::com::diag::hayloft::s3::Session::USER_AGENT_ENV(), optarg, !0);
            break;
        case '!':
            ::setenv(::com::diag::hayloft::Logger::MASK_ENV(), "0xfff0", !0);
            Logger::instance().setMask();
            break;
        case '?':
            usage(program, stderr);
            break;
        default:
        	usage(program, stderr);
            ::exit(1);
            break;
        }
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

	fprintf(stderr, "Complex example starting.\n");
    if (complexity()) {
    	fprintf(stderr, "Complex example succeeded.\n");
    } else {
    	fprintf(stderr, "Complex example FAILED!\n");
    	xc = 6;
    }

    ::exit(xc);
}
