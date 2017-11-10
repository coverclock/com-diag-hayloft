#!/bin/bash
. out/host/bin/setup
if [[ $# -eq 0 ]]; then
	unset COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE
	unset COM_DIAG_HAYLOFT_DEBUG
elif [[ "$1" == "-d" ]]; then
	export COM_DIAG_HAYLOFT_LIBS3_CURL_VERBOSE=1
	export COM_DIAG_HAYLOFT_DEBUG=1
else
	:
fi
exec out/host/got/unittest
