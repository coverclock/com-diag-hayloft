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
	explicit MyObjectGet(const Object & object, const Plex & plex, const char * outputname)
	: ObjectGet(object, plex, new PathOutput(outputname))
	, path(outputname)
	{}
	virtual bool reset(bool force = false) {
		return ObjectGet::reset(new PathOutput(path), 0, 0, force);
	}
};

static bool synchronicity() {
	return true;
}

static bool simplexity() {
	return true;
}

static bool multiplexitycomplete() {
	return true;
}

static bool multiplexityservice() {
	return true;
}

static bool complexity() {
	bool result = false;

	do {

		const char * PATH = "unittest.txt";
		Bucket BUCKET1("Complexity1");
		Bucket BUCKET2("Complexity2");
		Object OBJECT1("Object1.txt", BUCKET1);
		Object OBJECT2("Object2.txt", BUCKET2);
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

    if (!synchronicity()) { ::exit(2); }
    if (!simplexity()) { ::exit(3); }
    if (!multiplexitycomplete()) { ::exit(4); }
    if (!multiplexityservice()) { ::exit(5); }
    if (!complexity()) { ::exit(6); }

}
