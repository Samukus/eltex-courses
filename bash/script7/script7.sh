#!/bin/bash 
# arg 1 = directory for backup
echo "Укажите файл(ы) для бэкапа"
read files
echo "Укажите метод : 1) tar 2) dd 3) git"
read method

echo "Укажите время (минуты,часы,день недели,месяц,число месяца)"
read minute
read hour
read dayofweek
read month
read day

if ! [ -d /home/$USER/backup ]; then
cd /home/$USER
mkdir backup
fi

if [ $method = 1 ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek tar -cf $1-`eval date +%F`.tar $files" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

if [ $method = 2 ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek dd if=$files of=$1-`eval date +%F`.dd" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

if [ $method = 3 ]; then
crontab -l > /tmp/crontab.file 
cp /tmp/crontab.file /tmp/crontab.file.backup
echo "$minute $hour $day $month $dayofweek `pwd`/git.sh $1 files" >> /tmp/crontab.file
crontab /tmp/crontab.file
exit 0
fi

exit 1
