#!/bin/bash

#time to reload info, s 
UPDATE_TIME=5	

if [[ $# == 1 ]]
then 
	UPDATE_TIME=$1
fi 


filter_equal (){
while [ 1 ]
do
	clear
	top -n 1 | awk -v pattern=$1 -v column=$2 '{if( $column ~ pattern || NR <= 7 ) {print $0}}'
	sleep $UPDATE_TIME
done
}

filter_less (){
while [ 1 ]
do
	clear
	top -n 1 | awk -v pattern=$1 -v column=$2 '{if( $column <= pattern || NR <= 7 ) {print $0}}'
	sleep $UPDATE_TIME
done
}

filter_more (){
while [ 1 ]
do
	clear
	top -n 1 | awk -v pattern=$1 -v column=$2 '{if( $column >= pattern || NR <= 7 ) {print $0}}'
	sleep $UPDATE_TIME
done
}

make_choice () {
	echo "Выберите фильтрацию меньше, больше или равно заданному значению ( type: '< значение' , '> значение' , '== значение' )"
	read CHOICE	
	 SIGN=`echo $CHOICE | awk '{print $1}'`
	VALUE=`echo $CHOICE | awk '{print $2}'`
	if [[ $SIGN == "<" ]]
	then
		filter_less $VALUE $1
	fi		
	if [[ $SIGN == "==" ]]
	then
		filter_equal $VALUE $1
	fi		
	if [[ $SIGN == ">" ]]
	then
		filter_more $VALUE $1
	fi	
}

clear
echo "Выберите фильтр для вывода данных команды top: "
echo 
echo "1< Фильтр по имени пользователя -----------------------------[USER]"
echo "2< Фильтр по колличеству используемой памяти (VIRT kB) ------[VIRT]"
echo "3< Фильтр по колличеству используемой памяти (RES  kB) -------[RES]"
echo "4< Фильтр по колличеству используемой памяти (SHR  kB) -------[SHR]"
#echo "5< Фильтр по загрузке процессора ----------------------------[%CPU]"
#echo "6< Фильтр по загрузке памяти --------------------------------[%MEM]"
echo "5< Фильтр по команде, запустившей процесс ----------------[COMMAND]"
echo "6< Фильтр по текущему приоритету процесса ---------------------[PR]"
echo "7< Фильтр по NICE приоритету процеса --------------------------[NI]"
echo "8< ФИльтр по идентефикатору процесса -------------------------[PID]"
echo "0 < Обычный вывод, без применения фильтров" 
echo

read filter

case $filter in
	0	)
		while [ 1 ]
		do
			clear
			top -n 1
			sleep $UPDATE_TIME
		done		
	;;
	1	)
		clear
		echo "Фильтр по имени пользователя!"
		echo
		echo "Введите имя пользователя, для которого необходимо выводить информацию:"
		read USERNAME			
		filter_equal $USERNAME 3
	;;
	2	)
		clear
		echo "Фильтр по колличеству используемой памяти (VIRT kB)"
		make_choice 6
	;;
	3	)
		clear
		echo "Фильтр по колличеству используемой памяти (RES  kB)"
		make_choice 7
	;;
	4	)
		clear
		echo "Фильтр по колличеству используемой памяти (SHR  kB)"
		make_choice 8
	;;
	5	)
		clear
		echo "Фильтр по команде, запустившей процесс"
		echo "Введите команду, для которой необходимо выводить информацию:"
		read COMM		
		filter_equal $COMM 13
	;;
	6	)
		clear
		echo "Фильтр по текущему приоритету процесса"
		make_choice 4
		
	;;
	7	)
		clear
		echo "Фильтр по NICE приоритету процесса"
		make_choice 5
	;;
	8	)
		clear
		echo "Фильтр по идентефикатору процесса"
		read PID
		filter_equal $PID 2
	;;
esac









