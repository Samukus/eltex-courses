MODNAME = nl_mod

obj-m += $(MODNAME).o

all:
	make -w -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o NetLinkSender NetLinkSender.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-sudo rmmod $(MODNAME).ko
	rm NetLinkSender 

install:
	-sudo rmmod $(MODNAME).ko
	sudo insmod $(MODNAME).ko
	#clear
	tail -10 /var/log/syslog
	./NetLinkSender
	./NetLinkSender GetRandom
