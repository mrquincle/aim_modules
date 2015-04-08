#!/bin/bash

file="Dirichlet Mixture Model Clusters [$(date)]"
echo "Use \"$file\" as temporarily file"

cpath=$(pwd)

cd ../builds/standard
./DirichletModule 0 | tee "${file}.out"
cat "${file}.out" | grep mean | cut -f5 -d':' > "${file}.txt"
rm output.octave
ln -s "${file}.txt" output.octave 

cd "$cpath"

octave clip.m 
