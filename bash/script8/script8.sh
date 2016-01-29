#!/bin/bash 

source ./configuration.sh

echo $files;

if ! [ -d /home/$USER/backup ]; then
cd /home/$USER
mkdir backup
fi

if [ $method = "1" ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek tar -cf $bdir-`eval date +%F`.tar $files" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

if [ $method = "2" ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek dd if=$files of=$bdir-`eval date +%F`.dd" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

if [ $method = "3" ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek `pwd`/git.sh $bdir $files" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

exit 1
