#!/bin/sh

i="0"

cnt="100"

echo "First run ./run.sh"
./init.sh

while [ $i -lt $cnt ]
do
	./turn_on.sh
	sleep 0.5
	./turn_off.sh
	i=$[$i+1]
done


