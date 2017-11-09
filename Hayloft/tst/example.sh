#!/bin/bash
. out/host/bin/setup
if [[ $# -eq 0 ]]; then
	unset COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE
	unset COM_DIAG_HAYLOFT_DEBUG
	DEBUG=""
elif [[ "$1" == "-d" ]]; then
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE=1
	export COM_DIAG_HAYLOFT_DEBUG=1
	DEBUG="-d"
else
	DEBUG=""
fi
# Unfortunately this approach to setting the id and secret cause them
# to show up in the process list when doing a "ps" command. So be aware
# of this when running on a shared server.
out/host/bin/example \
	-i $(cat $HOME/projects/hayloft/aws-s3-access-key-id.txt) \
	-k $(cat $HOME/projects/hayloft/aws-s3-secret-access-key.txt) \
	-u $(cat $HOME/projects/hayloft/aws-s3-user-agent.txt) \
	-b -example-hayloft-diag-com \
	${DEBUG}
