#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x21fafc8f, "device_destroy" },
	{ 0x5fcb3cb9, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x92997ed8, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6fb1b627, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xd7fee422, "__platform_driver_register" },
	{ 0x6150d0ae, "platform_driver_unregister" },
	{ 0x63b550de, "class_destroy" },
	{ 0x9594d294, "kmalloc_caches" },
	{ 0x4071fce3, "kmalloc_trace" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xf5847324, "cdev_init" },
	{ 0x57204e4c, "cdev_add" },
	{ 0x16dcc089, "device_create" },
	{ 0xc6308436, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B2044DFA91D433A80EBC938");
