#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/device/class.h>
#include <linux/types.h>
#define MAX_SIZE_BUFF 512

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

char device_buff[MAX_SIZE_BUFF] = {0,};
/* hold the device number */
dev_t dev_num;

/*Cdev variable*/
struct cdev pcd_cdev;

/*pcd_open module*/
int pcd_open(struct inode *iNode ,struct file *File)
{
	pr_info("this pcd open module");
	/*int ret = check_permission();
	if(!ret)
	{
		pr_info("Open successfully");
	}
	else
	{
		pr_err("Open failed");
	}*/
	return 0;
}	

/*pcd_release module*/
int pcd_release(struct inode *iNode ,struct file *File)
{
        pr_info("this pcd release module");
        return 0;
} 


/*pcd_read module*/
ssize_t pcd_read(struct file *File,char __user *buff,size_t count, loff_t *fos) 
{
	if(*fos+count > MAX_SIZE_BUFF)
	       count = MAX_SIZE_BUFF - *fos;
         if(copy_to_user(buff,&device_buff[*fos],count))
	 {
		 return -EFAULT;
	 }	 
	 *fos += count;
	 pr_info("Number of bytes successfully read %zu\n",count);
	 pr_info("updated file position = %lld\n",*fos);
	 return count;
}	

/*pcd_write module*/
ssize_t pcd_write(struct file *File,const char __user *buff,size_t count, loff_t *fos)
{
        if(*fos+count > MAX_SIZE_BUFF)
               count = MAX_SIZE_BUFF - *fos;
	if(!count)
		return ENOMEM;
         if(copy_from_user(&device_buff[*fos],buff,count))
         {
                 return -EFAULT;
         }
         *fos += count;
         pr_info("Number of bytes successfully read %zu\n",count);
         pr_info("updated file position = %lld\n",*fos);
         return count;
} 



/*file operation of the driver*/
struct file_operations pcd_fops= 
{
	.read  = pcd_read,
	.write = pcd_write,
	.open  = pcd_open,
	.release = pcd_release,
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
	ret = alloc_chrdev_region(&dev_num,0,1,"Jitu_device_pcd");
	if (ret < 0)
	{
		pr_err("Alloc chrdev Failed");
		goto out; 	
	}
       	int minor = MINOR(dev_num);
	int majar = MAJOR(dev_num);
	pr_info("Major number %u:",majar);
        pr_info("Minor number %u:",minor);	
        
	/*2 Initialize the cdev structure with fops*/
	cdev_init(&pcd_cdev,&pcd_fops);

	/*3 Register a device (cdev structure) with VFS*/
  	pcd_cdev.owner = THIS_MODULE;
	ret = cdev_add(&pcd_cdev,dev_num,1);
        if(ret < 0)
	{
		goto unreg_char;
	}
    	/*4 create class in /sys/class/*/
	class_pcd = class_create(THIS_MODULE,"Jitu_etx_Dev");
        if (IS_ERR(class_pcd))
        {
		pr_err("Failed to create class");
		ret = PTR_ERR(class_pcd);
		goto cdev_del;
        }

	/*5 create device in sysfs */
	device_pcd = device_create(class_pcd,NULL,dev_num,NULL,"Jitu_pcd_device");
	if (IS_ERR(device_pcd))
        {
                pr_err("Failed to create device");
		ret = PTR_ERR(device_pcd);
		goto class_del;
        }

	pr_info("Module successfully init\n");

class_del:
      	class_destroy(class_pcd);
cdev_del:
	cdev_del(&pcd_cdev);
unreg_char:
       	unregister_chrdev_region(dev_num,1);
out:
       
	pr_err("Module inseration Failed");
	return ret;
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

