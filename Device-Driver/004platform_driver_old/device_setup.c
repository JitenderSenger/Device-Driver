#include<linux/module.h>
#include<linux/platform_device.h>
#include"platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

void pcd_release(struct device *dev)
{
	pr_info("Release pcd device");
}

/* create 2 platform device data structure*/
struct pcddev_platform_data pcdev_pdata[2] =
{
	[0] = { .size = 512, .perm = RDONLY, .serial_number = "PCD1111" },
	[1] = { .size = 1024, .perm = WRONLY, .serial_number = "PCD2222" }
};

/* create 2 platform  device*/
struct platform_device pcd_platform_dev1 = {
	.name ="pcd_char_device",
	.id = 0,
	.dev = { .platform_data = &pcdev_pdata[0], .release = &pcd_release}
};


struct platform_device pcd_platform_dev2 = {
        .name ="pcd_char_device",
        .id = 1,
	.dev = { .platform_data = &pcdev_pdata[1], .release = &pcd_release}

};



static int __init pcd_platform_init(void)
{
	pr_info("PCD platform device init module");
	platform_device_register(&pcd_platform_dev1);
	platform_device_register(&pcd_platform_dev2);
        pr_info("PCD platform device init module successfully");
	return 0;
}	

static void __exit pcd_platform_exit(void)
{
	pr_info("PCD platform exit module");
	platform_device_unregister(&pcd_platform_dev1);
	platform_device_unregister(&pcd_platform_dev2);
	pr_info("PCD platform device remove");
}

module_init(pcd_platform_init);
module_exit(pcd_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger");
MODULE_DESCRIPTION("pcd_platform driver");
MODULE_VERSION("1.0.1");
                          
