#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xb543352, "device_destroy" },
	{ 0xea81176e, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x122c3a7e, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xee925817, "class_create" },
	{ 0xaa59264c, "__platform_driver_register" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x33326c96, "platform_driver_unregister" },
	{ 0x479e03d7, "class_destroy" },
	{ 0xab69f068, "kmalloc_caches" },
	{ 0x5c6fcbb3, "kmalloc_trace" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x14bd5970, "cdev_init" },
	{ 0xeab4ef9c, "cdev_add" },
	{ 0xe673f7e9, "device_create" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x6ab589bc, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "865C9E153E474335011C0F0");
