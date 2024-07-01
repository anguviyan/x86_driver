#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int myint = 10;
module_param(myint,int,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP);
MODULE_PARM_DESC(myint,"This is common integer value");

static short int myshort = 2;
module_param(myshort,short,S_IRUSR|S_IWUSR|S_IXUSR|S_IWGRP);
MODULE_PARM_DESC(myshort,"This is short integer value");

static long int mylong = 1212;
module_param(mylong,long,0000);
MODULE_PARM_DESC(mylong,"This is long integer value");

static char *name = "sriram";
module_param(name,charp,0000);
MODULE_PARM_DESC(name,"This is string value");

static int arr_val[2] = {-1,1};
static int arr_arg = 0;
module_param_array(arr_val,int,&arr_arg,0000);
MODULE_PARM_DESC(arr_val,"This is the array value");

static int __init start(void)
{
	int i;
	pr_info("The value of integer is %d\n",myint);
	pr_info("The value of short is %d\n",myshort);
	pr_info("The value of long is %ld\n",mylong);
	pr_info("The value of the string is %s\n",name);
	for(i=0;i<((sizeof(arr_val)/sizeof(int)));i++)
		pr_info("The value of arr_val[%d] is %d\n",i,arr_val[i]);
	pr_info("The number of argument passed while invoke the program is %d\n",arr_arg);
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
