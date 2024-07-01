#include <linux/kernel.h>
#include <linux/module.h>

static int __init start(void)
{
	pr_info("Hello World\n");
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye..\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
