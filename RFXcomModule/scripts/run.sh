#!/bin/sh

middleware=yarp
id=0

yarpserver --read &

# Stop running processes on exit of script
cleanup() {
	local pids=$(jobs -pr)
	[ -n "$pids" ] && kill $pids
}
trap "cleanup" INT QUIT TERM EXIT

cd .. 

make

cd builds/$middleware && ./RFXcomModule $id
