#include <linux/kernel.h>
#include <linux/module.h>

int value = 555;
void func(void);
/*verify include/linux/export.h*/
EXPORT_SYMBOL_GPL(value);
EXPORT_SYMBOL_GPL(func);

void func(void)
{
	printk(KERN_INFO"The value is %d\n",value);
}

static int __init start(void)
{
	func();
	return 0;
}

static void __exit stop(void)
{
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");

