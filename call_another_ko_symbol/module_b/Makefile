PWD := $(shell pwd)
ALL_OBJS := demo_b.o
MODULE_NAME := module_b

$(MODULE_NAME)-y += $(ALL_OBJS)

EXTRA_CFLAGS += -g -O0

#KSRC := /usr/src/linux-headers-$(shell uname -r)

ARCH := csky
CROSS_COMPILE := /opt/gxtools/csky-linux-4.9.y/bin/csky-linux-
KSRC := /opt/goxceed/kernel-csky

KBUILD_EXTRA_SYMBOLS += $(PWD)/../module_a/Module.symvers
export KBUILD_EXTRA_SYMBOLS

module:
	#make -C $(KSRC) M=$(PWD)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd)  modules

obj-m := $(MODULE_NAME).o

clean:
	rm -fr Module.symvers ; rm -fr Module.markers ; rm -fr modules.order
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	rm -fr .tmp_versions
	rm -fr .cache.mk
