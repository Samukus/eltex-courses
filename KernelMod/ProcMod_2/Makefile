obj-m += random_proc.o
all:
	make -i clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod random_proc.ko
	clear
	tail -5 /var/log/syslog
	cat /proc/random_proc
	cat /proc/random_proc
	cat /proc/random_proc
	cat /proc/random_proc
	cat /proc/random_proc
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod random_proc.ko

