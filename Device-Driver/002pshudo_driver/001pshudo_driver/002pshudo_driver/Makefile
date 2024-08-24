obj-m :=Jitu_char_driver.o
HOST_KERN_DIR=/lib/modules/6.1.0-rpi8-rpi-2712/build/
all:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
host_clean:
	make -C $(HOST_KERN_DIR) M=$(PWD) clean

