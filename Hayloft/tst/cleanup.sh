#!/bin/bash
LIST=$(s3 list | tail -n +3 | cut -d ' ' -f 1 | grep -e '-unittest-hayloft-diag-com$')
for BUCKET in ${LIST}; do
	echo s3 delete ${BUCKET}
	s3 delete ${BUCKET}
done
