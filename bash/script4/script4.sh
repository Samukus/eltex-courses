#!/bin/bash

echo "This is $1 cheking script!"
while [[ 1 == 1 ]]
do	
	ps -fC $1 >/dev/null
	if [[ $? != 0 ]]
	then
		echo "no process $1 find"
		echo "starting $1..."
		$1 >/dev/null
	fi
	sleep 1
done
