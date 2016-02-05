MODNAME = sysfs_mod

obj-m += $(MODNAME).o

all:
	make -w -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-sudo rmmod $(MODNAME).ko

install:
	-sudo rmmod $(MODNAME).ko
	sudo insmod $(MODNAME).ko
	clear
	tail -2 /var/log/syslog
	cat /sys/$(MODNAME)/GetRandom

