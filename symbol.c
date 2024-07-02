#include <linux/module.h>
#include <linux/kernel.h>


int value;

void func(void)
{
	pr_info("The value is %d\n",value);
	value++;
	pr_info("The value is %d\n",value);
}

EXPORT_SYMBOL_GPL(value);
EXPORT_SYMBOL_GPL(func);

static int __init start(void)
{
	pr_info("we are in the symbol exporting program\n");
	func();
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
