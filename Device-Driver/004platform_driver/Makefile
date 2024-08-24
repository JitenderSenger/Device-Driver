obj-m :=device_setup.o pcd_platform_driver.o
HOST_KERN_DIR=/lib/modules/$(shell uname -r)/build/
all:
	make -C $(HOST_KERN_DIR) M=$(shell pwd) modules
host:
	make -C $(HOST_KERN_DIR) M=$(shell pwd) modules
host_clean:
	make -C $(HOST_KERN_DIR) M=$(shell pwd) clean

