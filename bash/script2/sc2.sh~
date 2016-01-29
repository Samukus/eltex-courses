#!/bin/bash

echo "Select path for script:"
read path
mkdir -p $path
cd $path
n1=9
n2=49
echo "Type a mask for files:"
echo "Example.."
echo "typed: 	var1 
		var2 
		var3"
echo "script mask: path/var1/var1_var2/var1_var2:var3"
read var1
read var2
read var3
echo "Input number of files:"
read count

for i in `seq 0 $n1`;
do
echo "$var1""$i"
mkdir "$var1""$i"
cd "$var1""$i"
for j in `seq 0 $n2`;
do
echo "$var1""$i"_"$var2""$j"
mkdir "$var1""$i"_"$var2""$j"
cd "$var1""$i"_"$var2""$j"
for k in `seq 0 $count`;
do
touch "$var1""$i"_"$var2""$j":"$var3""$k"
done
cd ..
done
cd ..
done
