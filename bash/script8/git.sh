#!/bin/bash
# 1) git folder 2) files to backup
# multiple files example "/file1 /file2 /file3"

echo -e "\n$1"
echo -e "\n$2\n"

if [ ! -e $1] 
then
	mkdir $1
	cd $1
	`git init`
fi

cp $2 $1
cd $1
COMMIT=`date +%F`_`date +%T`
`git add .`
`git commit -m $COMMIT`

exit 0

