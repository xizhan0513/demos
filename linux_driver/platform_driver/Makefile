PWD := $(shell pwd)
ALL_OBJS := platform_driver.o platform_device.o
MODULE_NAME_DRIVER := platform_driver
MODULE_NAME_DEVICE := platform_device

$(MODULE_NAME)-y += $(ALL_OBJS)

EXTRA_CFLAGS += -g -O0

#KSRC := /usr/src/linux-headers-$(shell uname -r)

ARCH := csky
CROSS_COMPILE := /opt/gxtools/csky-linux-4.9.y/bin/csky-linux-
KSRC := /home/xizhan/goxceed/v1.9-dev/linux-4.9.y

module:
	#make -C $(KSRC) M=$(PWD)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd) modules

obj-m := $(MODULE_NAME_DRIVER).o $(MODULE_NAME_DEVICE).o

clean:
	rm -fr Module.symvers ; rm -fr Module.markers ; rm -fr modules.order
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	rm -fr .tmp_versions
	rm -fr .cache.mk
