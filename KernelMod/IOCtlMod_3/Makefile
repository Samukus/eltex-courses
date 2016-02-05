obj-m += ioctl_mod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o ioctl.a ioctl.c

install:
	-sudo insmod ioctl_mod.ko
	-sudo mknod /dev/ioctl_random c 200 0
	-sudo chmod 777 /dev/ioctl_random
	tail -1 /var/log/syslog
	cat /dev/ioctl_random
	./ioctl.a

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-sudo rmmod ioctl_mod.ko
	-sudo rm /dev/ioctl_random
