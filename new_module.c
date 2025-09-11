/*Header section*/
#include <linux/kernel.h>
#include <linux/module.h>

/*This is module initialization entry point*/
static int __init start(void)
{
	/*Kernel printkf*/
	printk(KERN_INFO"Hai this is sriram\n");//include/linux/printk.h
	return 0;
}

/*This is module clean-up entry point*/
static void __exit stop(void)
{
	printk(KERN_INFO"Bye\n");
}

/*This is registration of above entry points with kernel*/
module_init(start);
module_exit(stop);

/*This is descriptive information about the module*/
MODULE_LICENSE("Dual BSD/GPL");
