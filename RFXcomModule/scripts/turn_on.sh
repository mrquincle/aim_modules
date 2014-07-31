#!/bin/sh

echo "16" | yarp write /write /rfxcommodule0/type0
echo "1" | yarp write /write /rfxcommodule0/select0
echo "1" | yarp write /write /rfxcommodule0/input0
