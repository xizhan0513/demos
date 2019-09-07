PWD := $(shell pwd)
ALL_OBJS := work_queue.o
MODULE_NAME := demo_workqueue

$(MODULE_NAME)-y += $(ALL_OBJS)

EXTRA_CFLAGS += -g -O0

#KSRC := /usr/src/linux-headers-$(shell uname -r)

ARCH := csky
CROSS_COMPILE := /opt/gxtools/csky-linux-4.9.y/bin/csky-linux-
KSRC := /opt/goxceed/kernel-csky

module:
	#make -C $(KSRC) M=$(PWD)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd)  modules

obj-m := $(MODULE_NAME).o

clean:
	rm -fr Module.symvers ; rm -fr Module.markers ; rm -fr modules.order
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	rm -fr .tmp_versions
