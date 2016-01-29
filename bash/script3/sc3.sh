#!/bin/bash

h=`date +'%H'`
m=`date +'%M'`

while [[ !(($m == "$2") && ($h == "$1")) ]];
do

h=`date +'%H'`
m=`date +'%M'`
sleep 1

done

echo "Пора вставать!"
mplayer signal.mp3

