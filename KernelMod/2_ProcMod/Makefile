obj-m += random_proc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod random_proc.ko

install:
	-sudo insmod random_proc.ko
	tail -5 /var/log/syslog
	cat /proc/random_proc
