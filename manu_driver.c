#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

#define MAJORNO	300
#define MINORNO	0
#define CHAR_NAME	"MY_DRIVER"
static dev_t first;
static int count = 1;

static int __init start(void)
{
	first = MKDEV(MAJORNO,MINORNO);                                       //include/linux/kdev_t.h
	if(register_chrdev_region(first,count,CHAR_NAME) < 0)                 //include/linux/fs.h
	{
		printk(KERN_INFO"This device is unregister from the kernel\n");
		return -1;
	}
	printk(KERN_INFO"This driver is register with the name %s\n",CHAR_NAME);
	printk(KERN_INFO"Major no %d and Minor no %d\n",MAJOR(first),MINOR(first));
	return 0;
}

static void __exit stop(void)
{
	unregister_chrdev_region(first,count);                                 //include/linux/fs.h
	printk(KERN_INFO"Driver is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
