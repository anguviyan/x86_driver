#include <linux/kernel.h>
#include <linux/module.h>

static int *ptr;

static int __init start(void)
{
	ptr= (int*)0xffffffffc0ec9380;
	*ptr = 111;
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye..\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
