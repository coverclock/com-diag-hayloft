#!/bin/bash
exec out/host/bin/example \
	-d \
	-i $(cat $HOME/projects/hayloft/aws-s3-access-key-id.txt) \
	-k $(cat $HOME/projects/hayloft/aws-s3-secret-access-key.txt) \
	-u $(cat $HOME/projects/hayloft/aws-s3-user-agent.txt) \
	-b $(cat $HOME/projects/hayloft/aws-s3-bucket-suffix.txt)
