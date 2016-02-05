obj-m += random.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod random.ko
	sudo rm /dev/mydev

install:
	-sudo insmod random.ko
	-sudo mknod /dev/mydev c 198 0
	tail -2 /var/log/syslog
	cat /dev/mydev
