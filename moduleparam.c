#include <linux/module.h>
#include <linux/kernel.h>

static short int svalue = 9;
module_param(svalue,short,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP); /*verify the uapi/linux/stat.h*/
MODULE_PARM_DESC(svalue,"This will be displayed in the /var/log/kern.log");

static int value = 99;
module_param(value,int,S_IRUSR|S_IWUSR|S_IRGRP);
MODULE_PARM_DESC(value,"This value will be displayed in the /var/log/kern.log");

static long int lvalue = 999;
module_param(lvalue,long,S_IRUSR);
MODULE_PARM_DESC(lvalue,"This value will be displayed in the /var/log/kern.log");

static char *name = "sriram";
module_param(name,charp,0000);
MODULE_PARM_DESC(name,"This name will be displayed in the /var/log/kern.log");

static int array[2] = {-1,1};
static int arg_arr = 0;
module_param_array(array,int,&arg_arr,0000);
MODULE_PARM_DESC(array,"This array will be displayed in the /var/log/kern.log");

static int __init start(void)
{
	int i = 0;
	printk(KERN_INFO"Module parameter based module\n");//include/linux/kern_levels.h
	printk(KERN_INFO"Short value is %d\n",svalue); //Documentation/core-api/printk-formats.rst
	printk(KERN_INFO"Int value is %d\n",value);
	printk(KERN_INFO"Long value is %ld\n",lvalue);
	printk(KERN_INFO"string is %s\n",name);
	for(i=0; i<(sizeof(array)/sizeof(int)); i++)
	{
		printk(KERN_INFO"Array[%d] value is %d\n",i,array[i]);
	}
	printk(KERN_INFO"The number of argument passed is %d\n",arg_arr);
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
