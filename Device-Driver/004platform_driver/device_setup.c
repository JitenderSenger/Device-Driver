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
struct pcddev_platform_data pcdev_pdata[4] =
{
	[0] = { .size = 512, .perm = RDONLY, .serial_number = "PCD1111" },
	[1] = { .size = 1024, .perm = WRONLY, .serial_number = "PCD2222" },
 	[2] = { .size = 256, .perm = RDONLY, .serial_number = "PCD3333" },
        [3] = { .size = 2048, .perm = RDWR, .serial_number = "PCD4444" }

};

/* create 2 platform  device*/
struct platform_device pcd_platform_dev1 = {
	.name ="pcdev-A1x",
	.id = 0,
	.dev = { .platform_data = &pcdev_pdata[0], .release = &pcd_release}
};


struct platform_device pcd_platform_dev2 = {
        .name ="pcdev-B1x",
        .id = 1,
	.dev = { .platform_data = &pcdev_pdata[1], .release = &pcd_release}

};

struct platform_device pcd_platform_dev3 = {
        .name ="pcdev-C1x",
        .id = 2,
        .dev = { .platform_data = &pcdev_pdata[2], .release = &pcd_release}

};

struct platform_device pcd_platform_dev4 = {
        .name ="pcdev-D1x",
        .id = 3,
        .dev = { .platform_data = &pcdev_pdata[3], .release = &pcd_release}

};

struct platform_device *pcd_devices[] =
{
	&pcd_platform_dev1,
	&pcd_platform_dev2,
	&pcd_platform_dev3,
	&pcd_platform_dev4

};

static int __init pcd_platform_init(void)
{
	pr_info("PCD platform device init module");
	//platform_device_register(&pcd_platform_dev1);
	//platform_device_register(&pcd_platform_dev2);
	platform_add_devices(pcd_devices,ARRAY_SIZE(pcd_devices));
        pr_info("PCD platform device init module successfully");
	return 0;
}	

static void __exit pcd_platform_exit(void)
{
	pr_info("PCD platform exit module");
	platform_device_unregister(&pcd_platform_dev1);
	platform_device_unregister(&pcd_platform_dev2);
	platform_device_unregister(&pcd_platform_dev3);
        platform_device_unregister(&pcd_platform_dev4);

	pr_info("PCD platform device remove");
}

module_init(pcd_platform_init);
module_exit(pcd_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger");
MODULE_DESCRIPTION("pcd_platform driver");
MODULE_VERSION("1.0.1");
                          
