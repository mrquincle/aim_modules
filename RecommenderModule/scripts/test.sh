#!/bin/bash

# temporarily
cd ..
aimmake
cd scripts

if [ -n "${COLORTERM}" ]; then
	terminal=$COLORTERM
else
	terminal=$TERM
fi

echo "Open terminal with $terminal"

#$terminal -x yarp server 

#sleep 1
#$terminal -x ../builds/yarp/RecommenderModule 0

yarp write /write verbatim /recommendermodule0/document <<< "doc0 yes yes no maybe"

sleep 2
yarp write /write verbatim /recommendermodule0/document <<< "doc1 yes yes yes yes maybe perhaps"

#sleep 1
#yarp write /write verbatim /recommendermodule0/document <<< "doc2 yes maybe maybe maybe"

$terminal -x yarp read /read verbatim /recommendermodule0/recommendation

sleep 1
yarp write /write verbatim /recommendermodule0/term <<< "doc0 yes"

sleep 1
yarp write /write verbatim /recommendermodule0/term <<< "doc0 no"

