obj-m	:= buggymod.o

KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm *.ko
	rm *.o
	rm *.mod.c
	rm modules.order
	rm Module.symvers
	rm -rf .tmp_versions
