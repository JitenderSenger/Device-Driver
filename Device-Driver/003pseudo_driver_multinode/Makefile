obj-m :=pcd_multinode.o
HOST_KERN_DIR=/lib/modules/6.5.0-25-generic/build/
all:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
host_clean:
	make -C $(HOST_KERN_DIR) M=$(PWD) clean

