#include <linux/kernel.h>
#include <linux/module.h>

extern int value;
extern void func(void);

static int __init start(void)
{
	func();
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
