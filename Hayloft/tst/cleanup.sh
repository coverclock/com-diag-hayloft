#!/bin/bash

export S3_ACCESS_KEY_ID=$(cat $HOME/projects/hayloft/aws-s3-access-key-id.txt)
export S3_SECRET_ACCESS_KEY=$(cat $HOME/projects/hayloft/aws-s3-secret-access-key.txt)
export PATH=$HOME/src/libs3-2.0/build/bin:$PATH

LIST1=$(s3 list | tail -n +3 | cut -d ' ' -f 1 | grep -e '-unittest-hayloft-diag-com$')
for BUCKET in ${LIST1}; do
	echo s3 delete ${BUCKET}
	s3 delete ${BUCKET}
done
LIST2=$(s3 list | tail -n +3 | cut -d ' ' -f 1 | grep -e 'test.diag.com$')
for BUCKET in ${LIST2}; do
	echo s3 delete ${BUCKET}
	s3 delete ${BUCKET}
done
LIST3=$(s3 list | tail -n +3 | cut -d ' ' -f 1 | grep -e '-example-hayloft-diag-com$')
for BUCKET in ${LIST3}; do
	echo s3 delete ${BUCKET}
	s3 delete ${BUCKET}
done
