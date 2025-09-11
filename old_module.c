#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void) //verify the include/linux/module.h
{
	printk(KERN_INFO"Hai this is sriram\n");//include/linux/kern_levels.h
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO"Bye\n");
}

MODULE_LICENSE("Dual BSD/GPL");
