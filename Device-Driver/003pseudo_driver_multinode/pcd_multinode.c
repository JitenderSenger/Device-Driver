#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/device/class.h>
#include <linux/types.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

#define NO_OF_DEVICE       4
#define MAX_SIZE_BUFF_1  128
#define MAX_SIZE_BUFF_2  256
#define MAX_SIZE_BUFF_3  512
#define MAX_SIZE_BUFF_4 1024


char device_buff_pcd_1[MAX_SIZE_BUFF_1] = {0,};
char device_buff_pcd_2[MAX_SIZE_BUFF_2] = {0,};
char device_buff_pcd_3[MAX_SIZE_BUFF_3] = {0,};
char device_buff_pcd_4[MAX_SIZE_BUFF_4] = {0,};

#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11

struct pcdev_private_data
{
	char *buffer;
	unsigned size;
        const char *serial_number;
        int perm;
       	struct cdev pcd_cdev;	
};

struct pcdrv_private_data
{
	int total_device;
	
	/* hold the device number */
	dev_t dev_num;
  	/*class obj*/
	struct class *class_pcd;
	/*device obj*/
	struct device *device_pcd;

	struct pcdev_private_data pcdev_data[NO_OF_DEVICE];
};

struct pcdrv_private_data pcdir_data =
{	
	.total_device = NO_OF_DEVICE,
	.pcdev_data   = {
		[0]   = {
				.buffer = device_buff_pcd_1,
				.size   = MAX_SIZE_BUFF_1,
				.serial_number = "PCD0x001",
				.perm = 0x01     //RDONLY
		        },
		[1]   = {
                                .buffer = device_buff_pcd_2,
                                .size   = MAX_SIZE_BUFF_2,
                                .serial_number = "PCD0x002",
                                .perm = 0x10     //WRONLY
                        },
		[2]   = {
                                .buffer = device_buff_pcd_3,
                                .size   = MAX_SIZE_BUFF_3,
                                .serial_number = "PCD0x003",
                                .perm = 0x11     //RDWR
                        },
		[3]   = {
                                .buffer = device_buff_pcd_4,
                                .size   = MAX_SIZE_BUFF_4,
                                .serial_number = "PCD0x004",
                                .perm = 0x01     //RDONLY
                        },
	}

};

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
int pcd_open(struct inode *iNode ,struct file *File)
{
	pr_info("this pcd open module");

	/*calling of open file operation*/
	int minor_n =  MINOR(iNode->i_rdev);

	struct pcdev_private_data *pcdev_data;
         
	pr_info("Minor of open file %u:",minor_n);

	/* get private data structure*/
	pcdev_data = container_of(iNode->i_cdev,struct pcdev_private_data,pcd_cdev);
        File->private_data = pcdev_data;
	int ret = check_permission(pcdev_data->perm,File->f_mode);
	if(!ret)
	{
		pr_info("Open successfully");
	}
	else
	{
		pr_err("Open failed");
	}
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
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)File->private_data;
	int max_size = pcdev_data->size;
	if(*fos+count > max_size)
	       count = max_size - *fos;
         if(copy_to_user(buff,pcdev_data->buffer+(*fos),count))
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
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)File->private_data;
        int max_size = pcdev_data->size;

        if(*fos+count > max_size)
               count = max_size - *fos;
	if(!count)
		return ENOMEM;
         if(copy_from_user(pcdev_data->buffer+(*fos),buff,count))
         {
                 return -EFAULT;
         }
         *fos += count;
         pr_info("Number of bytes successfully read %zu\n",count);
         pr_info("updated file position = %lld\n",*fos);
         return count;
} 

loff_t pcd_lseek(struct file *File,loff_t fos, int whence)
{

        struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)File->private_data;
        int max_size = pcdev_data->size;
        pr_info("Current file postion->%lld",File->f_pos);
	loff_t temp;
	switch(whence)
	{
		case SEEK_CUR:
		       	if((fos > max_size ) || (fos < 0))
				return -EINVAL;	
			File->f_pos = fos;
			break;
		case SEEK_SET:
		        temp = File->f_pos + fos;
		        if((temp > max_size ) || (temp < 0))
                                return -EINVAL;
			File->f_pos = temp;
			break;
		case SEEK_END:
		        temp = max_size + fos;
                        if((temp > max_size ) || (temp < 0))
                                return -EINVAL;
                        File->f_pos = temp;
                        break;

		default:
			return -EINVAL;
	}
	pr_info("New file postion->%lld",File->f_pos);

	return File->f_pos;

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



/*This is init entry point*/
static int __init pcd_module_init(void)
{
	int ret,index=0;
	pr_info("Hello world, This is init module\n");
	/*1 Dynamically allocate a device number*/
	ret = alloc_chrdev_region(&pcdir_data.dev_num,0,NO_OF_DEVICE,"pcd_devs");
	if (ret < 0)
	{
		pr_err("Alloc chrdev Failed");
		goto out; 	
	}
	pr_info("Minor number %u:",pcdir_data.dev_num);

	/*2 create class in /sys/class/*/
        pcdir_data.class_pcd = class_create("pcd_etx_Dev");
       	if (IS_ERR(pcdir_data.class_pcd))
        {
                pr_err("Failed to create class");
                ret = PTR_ERR(pcdir_data.class_pcd);
    		goto cdev_del;
	}
		
	for(index = 0; index < NO_OF_DEVICE ; index++)
	{
       		int minor = MINOR(pcdir_data.dev_num+index);
		int majar = MAJOR(pcdir_data.dev_num+index);
		pr_info("Major number %u:",majar);
        	pr_info("Minor number %u:",minor);	
	
		/*3 Initialize the cdev structure with fops*/
		cdev_init(&pcdir_data.pcdev_data[index].pcd_cdev,&pcd_fops);

		/*4 Register a device (cdev structure) with VFS*/
  		pcdir_data.pcdev_data[index].pcd_cdev.owner = THIS_MODULE;
		ret = cdev_add(&pcdir_data.pcdev_data[index].pcd_cdev,pcdir_data.dev_num+index,1);
        	if(ret < 0)
		{
			pr_err("Failed to add device");
                        pr_err("%s:%d error code %d\n", __func__, __LINE__, ret);
			goto cdev_del;
		}

		/*5 create device in sysfs */
		pcdir_data.device_pcd = device_create(pcdir_data.class_pcd,NULL,pcdir_data.dev_num+index,NULL,"pcd_device_%d",(index+1));
		if (IS_ERR(pcdir_data.device_pcd))
        	{
                	pr_err("Failed to create device");
		 	pr_err("error code %d\n", PTR_ERR(pcdir_data.device_pcd));
			ret = PTR_ERR(pcdir_data.device_pcd);
			goto class_del;
        	}
		pr_info("pcd_device_%u Module successfully init\n",index+1);

	
	}
	pr_info("Module successfully init\n");
	return 0;

cdev_del:
class_del:
	for(index = (NO_OF_DEVICE-1);index>=0;index--)
	{
		device_destroy(pcdir_data.class_pcd,pcdir_data.dev_num+index);
		cdev_del(&pcdir_data.pcdev_data[index].pcd_cdev);
	}
	class_destroy(pcdir_data.class_pcd);

unreg_char:
       	unregister_chrdev_region(pcdir_data.dev_num,NO_OF_DEVICE);
out:
       
	pr_err("Module inseration Failed");
	return ret;
}

/* this is cleanup  entry point*/

static void __exit pcd_module_exit(void)
{
	pr_info("This is exit module\n");
        int index=0;
	for(index=0;index<NO_OF_DEVICE;index++)
        {
                device_destroy(pcdir_data.class_pcd,pcdir_data.dev_num+index);
                cdev_del(&pcdir_data.pcdev_data[index].pcd_cdev);
        }
        class_destroy(pcdir_data.class_pcd);

        unregister_chrdev_region(pcdir_data.dev_num,NO_OF_DEVICE);

	pr_info("Module successfully unloaded\n");
}

module_init(pcd_module_init);
module_exit(pcd_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger -1");
MODULE_DESCRIPTION("pcd_multinode-1");
MODULE_VERSION("1.0.2");
