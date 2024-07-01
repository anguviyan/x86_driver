#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
	pr_info("Hai\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("Bye...\n");
}

MODULE_LICENSE("Dual BSD/GPL");
