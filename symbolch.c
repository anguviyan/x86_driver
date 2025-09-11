#include <linux/kernel.h>
#include <linux/module.h>

static int *ptr;

static int __init start(void)
{
	ptr = (int*)__symbol_get("value");
	if(ptr)
	{
		*ptr = 666;
		__symbol_put("value");
	}
	else
	{
		printk(KERN_INFO"This value is not listed in the symbol table\n");
		return -EINVAL;
	}

	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
