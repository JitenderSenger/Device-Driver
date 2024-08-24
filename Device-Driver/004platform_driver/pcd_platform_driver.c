#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/device/class.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include "platform.h"

#define MAX_DEVICES 4

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

//private device data
struct pcddev_private_Data
{
	struct pcddev_platform_data pdev;
	char *buffer;
        dev_t dev_num;
        struct cdev pcdev;	
};

//private driver data
struct pcddir_private_Data
{
	int total_no_device;
	dev_t dev_num_base;
	struct class *class_pcd;
	struct device *device_pcd;
};

struct device_config
{
	int config_item1;
	int config_item2;
};


struct device_config pcdev_config[4] =
{
	[0] = { .config_item1 = 60 , .config_item2 = 21},
	[1] = { .config_item1 = 50 , .config_item2 = 22},
	[2] = { .config_item1 = 40 , .config_item2 = 23},
	[3] = { .config_item1 = 30 , .config_item2 = 24}
};

struct pcddir_private_Data pcddir_data;


/*check permission module*/

int check_permission(int perm, int accessMode)
{
        if(perm == RDWR)
                return 0;
        if((perm == RDONLY)&&( (accessMode == FMODE_READ) && !( accessMode == FMODE_WRITE)))
                return 0;
        if((perm == WRONLY)&&( (accessMode == FMODE_WRITE) && !( accessMode == FMODE_READ)))
                return 0;
        return -EPERM;
}

/*pcd_open module*/
int pcd_open(struct inode *iNode, struct file *File)
{
	return 0;
}

/*pcd_release module*/
int pcd_release(struct inode *iNode, struct file *File)	
{
	return 0;
}

/*pcd_read module*/
ssize_t pcd_read(struct file *File,char __user *buffer, size_t count, loff_t *fop)
{
	return 0;
}

/*pcd_write module*/
ssize_t pcd_write(struct file *File,const char __user *buffer, size_t count, loff_t *fop)
{
	return 0;
}

loff_t pcd_lseek(struct file *File, loff_t fop, int whence)
{
	return 0;
}
/*file operation of the driver*/
struct file_operations pcd_fops=
{
        .read  = pcd_read,
        .write = pcd_write,
        .open  = pcd_open,
        .llseek = pcd_lseek,
        .release = pcd_release,
        .owner = THIS_MODULE
};


/*get called when device is found*/
int pcddir_platform_probe(struct platform_device *pdev)
{
	int ret;
	struct pcddev_private_Data *dev_data;

	struct pcddev_platform_data *pdata;


	/*1. Get the platform data*/
	pdata =(struct pcddev_platform_data*) dev_get_platdata(&pdev->dev);
	if(!pdata)
	{
		pr_info("No platform device found");
		ret = -EINVAL;
		goto out;
	}

	/*2. Dynamically memory allocation for device private data*/
	dev_data = kzalloc(sizeof(dev_data),GFP_KERNEL);
	if(!dev_data)
	{
		pr_info("No memory allocate");
                ret = -ENOMEM;
                goto out;
	}
	pdev->dev.driver_data = dev_data;
	//dev_set_drvdata(*pdev->dev,dev_data);
	dev_data->pdev.size = pdata->size;
 	dev_data->pdev.perm = pdata->perm;
	dev_data->pdev.serial_number = pdata->serial_number;
        
	pr_info("Device serial number%u\n",dev_data->pdev.serial_number);
	pr_info("Device size%u\n",dev_data->pdev.size);
	pr_info("Device permission%u\n",dev_data->pdev.perm);
	
	pr_info("Config item 1 = %d\n",pcdev_config[pdev->id_entry->driver_data].config_item1);
	pr_info("Config item 2 = %d\n",pcdev_config[pdev->id_entry->driver_data].config_item2);
	
	/*3. Dynamically memory allocation for device buffer using size information
	 * from platform  data*/
	dev_data->buffer = kzalloc(dev_data->pdev.size,GFP_KERNEL);
        if(!dev_data)
        {
                pr_info("No memory allocate for buffer");
                ret = -ENOMEM;
                goto dev_data_free;
        }


	/*4. Get the device number*/
	dev_data->dev_num = pcddir_data.dev_num_base + pdev->id;
        /*5. Do cdev init and add*/
	
	cdev_init(&dev_data->pcdev,&pcd_fops);
        dev_data->pcdev.owner = THIS_MODULE;
	ret = cdev_add(&dev_data->pcdev,dev_data->dev_num,1);
        if(ret < 0)
        { 
		pr_err("Failed to add device");
		pr_err(" error code %d\n", ret);
		goto buffer_free;
	}


	/*6. Create device file for the detect platform device*/
	pcddir_data.device_pcd = device_create(pcddir_data.class_pcd,NULL,dev_data->dev_num,NULL,"pcd_device_%d",pdev->id);
        if (IS_ERR(pcddir_data.device_pcd))
        {
 		pr_err("Failed to create device");
                pr_err("error code %d\n", PTR_ERR(pcddir_data.device_pcd));
                ret = PTR_ERR(pcddir_data.device_pcd);
		goto cdev_del;
        }

	pcddir_data.total_no_device++;
	pr_info("The provb successfully");
	return 0;

/*7 error handling*/
cdev_del:
	cdev_del(&dev_data->pcdev);
buffer_free:
	kfree(dev_data->buffer);
dev_data_free:
	kfree(dev_data);
out:
	pr_info("Device probe fail");
	return ret;
}

/*get called when device remove*/
int pcddir_platform_remove(struct platform_device *pdev)
{
	struct pcddev_private_Data *dev_data = dev_get_drvdata(&pdev->dev);
	/*Remove  the device that was create device_create*/
	device_destroy(pcddir_data.class_pcd,dev_data->dev_num);
	/*Remove cdev entry from systm*/
	cdev_del(&dev_data->pcdev);
	/* Free memory*/
	kfree(dev_data->buffer);
	kfree(dev_data);
	pcddir_data.total_no_device--;
	pr_info("Device is removed");
        return 0;
}

enum pcdev_names
{	
	PCDEVA1X,
	PCDEVB1X,
	PCDEVC1X,
	PCDEVD1X
};


struct platform_device_id pcdevs_ids[] =
{
	[0] = { .name = "pcdev-A1x", .driver_data = PCDEVA1X},
	[1] = { .name = "pcdev-B1x", .driver_data = PCDEVB1X},
	[2] = { .name = "pcdev-C1x", .driver_data = PCDEVC1X},
	[3] = { .name = "pcdev-D1x", .driver_data = PCDEVD1X}

};

struct platform_driver pcddir_platform =
{
        .probe  = pcddir_platform_probe,
        .remove = pcddir_platform_remove,
	.id_table = pcdevs_ids,
        .driver = { .name = "pcd_char_device" }
};

/*This is init entry point*/
static int __init pcd_platform_driver_module_init(void)
{
	int ret;
	/*1 Dynamically allocate a device number*/
        ret = alloc_chrdev_region(&pcddir_data.dev_num_base,0,MAX_DEVICES,"pcd_devs");
        if (ret < 0)
        {
                pr_err("Alloc chrdev Failed");
                goto out;
        }

        /*2 create class in /sys/class/*/
        pcddir_data.class_pcd = class_create(THIS_MODULE,"pcd_plat_Dev");
        if (IS_ERR(pcddir_data.class_pcd))
        {
                pr_err("Failed to create class");
                ret = PTR_ERR(pcddir_data.class_pcd);
                goto cdev_del;
        }

	platform_driver_register(&pcddir_platform);
	pr_info("pcd platform driver is loaded");
	return 0;
cdev_del:
unreg_char:
        unregister_chrdev_region(pcddir_data.dev_num_base,MAX_DEVICES);
out:

        pr_err("Module inseration Failed");
        return ret;

}

/*This is exit entry point*/
static void __exit pcd_platform_driver_module_exit(void)
{
	platform_driver_unregister(&pcddir_platform);
	class_destroy(pcddir_data.class_pcd);

        unregister_chrdev_region(pcddir_data.dev_num_base,MAX_DEVICES);

        pr_info("pcd platform driver removed successfully");

}

module_init(pcd_platform_driver_module_init);
module_exit(pcd_platform_driver_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger_1");
MODULE_DESCRIPTION("pcd platform driver module");
MODULE_VERSION("1.0.2");

