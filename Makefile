obj-m	:= buggymod.o

SYSTEMMAP		:= System.map-$(shell uname -r)
TABLE_ADDRESS	:= $(shell grep "R sys_call_table" /boot/$(SYSTEMMAP) | cut -f1 -d' ')
KDIR			:= /lib/modules/$(shell uname -r)/build
PWD			:= $(shell pwd)

default:
	sed -i "s/__SYSCALL_TABLE_ADDRESS__/0x$(TABLE_ADDRESS)/" buggymod.c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm *.ko
	rm *.o
	rm *.mod.c
	rm .*.cmd
	rm modules.order
	rm Module.symvers
	rm -rf .tmp_versions
