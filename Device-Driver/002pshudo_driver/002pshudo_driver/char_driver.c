#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/types.h>
#define MAX_SIZE_BUFF 512
char device_buff[MAX_SIZE_BUFF] = {0,};
/* hold the device number */
dev_t dev_num;

/*Cdev variable*/
struct cdev pcd_cdev;

/*file operation of the driver*/
struct file_operations pcd_fops= 
{
    .owner = THIS_MODULE	
};


/*class obj*/
struct class *class_pcd;

/*device obj*/
struct device *device_pcd;

/*This is init entry point*/
static int __init hello_module_init(void)
{
	int ret;
	pr_info("Hello world, This is init module\n");
	/*1 Dynamically allocate a device number*/
	ret = alloc_chrdev_region(&dev_num,0,1,"pcd_device");
	int minor = MINOR(dev_num);
	int majar = MAJOR(dev_num);
	pr_info("Major number %u:",majar);
        pr_info("Minor number %u:",minor);	
        
	/*2 Initialize the cdev structure with fops*/
	//cdev_init(&pcd_cdev,&pcd_fops);

	/*3 Register a device (cdev structure) with VFS*/
  	//pcd_cdev.owner = THIS_MODULE;
	//cdev_add(&pcd_cdev,dev_num,1);

    	/*4 create class in /sys/class/*/
	//class_pcd = class_create("pcd_class");

	/*5 create device in sysfs */
	//device_pcd = device_create(class_pcd,NULL,dev_num,NULL,"pcd");

	pr_info("Module successfully init\n");

	return 0;
}

/* this is cleanup  entry point*/

static void __exit hello_module_exit(void)
{
	pr_info("This is exit module\n");

	device_destroy(class_pcd,dev_num);
	class_destroy(class_pcd);
	cdev_del(&pcd_cdev);
	unregister_chrdev_region(dev_num,1);
	
	pr_info("Module successfully unloaded\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger");
MODULE_DESCRIPTION("pseduo driver");
MODULE_VERSION("1.0.2");
