#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	pr_info("Hai\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("bye\n");
}

MODULE_LICENSE("Dual BSD/GPL");
