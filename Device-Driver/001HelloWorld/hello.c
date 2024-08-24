#include <linux/module.h>

/*This is init entry point*/
static int __init hello_module_init(void)
{
	pr_info("Hello world, This is init module");
	return 0;
}

/* this is cleanup  entry point*/

static void __exit hello_module_exit(void)
{
	pr_info("Good bye, this is exit module");
}

module_init(hello_module_init);
module_exit(hello_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jitender Senger");
MODULE_DESCRIPTION("A kernal module to print hello world");
