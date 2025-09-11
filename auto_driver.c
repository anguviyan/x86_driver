#include <linux/kernel.h>
#include <linux/module.h>

#define CHAR_NAME "MY_DRIVER"

static dev_t first;
static int minor = 0;
static int count = 1;

static int __init start(void)
{
	if(alloc_chrdev_region(&first, minor, count, CHAR_NAME) < 0)
	{
		printk(KERN_INFO"This driver is not register in the kernel\n");
		return -1;
	}
	printk(KERN_INFO"This driver is register in the kernel with name %s\n",CHAR_NAME);
	printk(KERN_INFO"Major no %d and Minor no %d\n",MAJOR(first),MINOR(first));
	return 0;
}

static void __exit stop(void)
{
	unregister_chrdev_region(first, count);
	printk(KERN_INFO"Driver is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
