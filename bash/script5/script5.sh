#!/bin/bash

find /var/log -maxdepth 1 -name "$1" | grep $1 >/dev/null

if [[ $? != 0 ]]
then
	exit 1
fi

for hour in `seq $2 $3`;
do 
	if [ $hour -lt 10 ]
	then
		cat /var/log/$1 | grep "\<0$hour:[0-9][0-9]:[0-9][0-9]\>" 
	else
		cat /var/log/$1 | grep "\<$hour:[0-9][0-9]:[0-9][0-9]\>"
	fi	
done

		



