#!/bin/bash


i=0
while [ 1 ]
do
	if [ "$i" -gt "10" ]
	then
		exit -1
	else
		i=$(($i + 1))
	fi	
	
	if ( mount | grep '/home/alex/shared/Server' >> /dev/null )
	then
		IP=`ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | cut -d: -f2 | awk '{printf $1"\n"}'`
		UP=` uptime | cut -d, -f1 `
		NAME="/media/alex/Server/log"
		echo "Alexey Skrobotov IP:$IP" >> $NAME
		echo "$UP" >> $NAME
		echo "" >> $NAME
		echo "" >> $NAME
		exit 0
	else
		sleep 1
	fi
done
