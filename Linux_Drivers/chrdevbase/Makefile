KERNELDIR := /home/tzz/source/linux-imx-rel_imx
CURRENT_PATH := $(shell pwd)
ENV := ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
obj-m := chrdevbase.o 

build: kernel_modules

kernel_modules: 
	$(MAKE) $(ENV) -C $(KERNELDIR) M=$(CURRENT_PATH) modules 
clean: 
	$(MAKE) $(ENV) -C $(KERNELDIR) M=$(CURRENT_PATH) clean 