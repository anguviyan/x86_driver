#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/atomic.h>

/*verify the linux/atomic/atomic-instrumented.h */
static int __init start(void)
{
	atomic_t av = ATOMIC_INIT(8);
	printk(KERN_INFO"Read the value %d\n",atomic_read(&av));
	atomic_add(10,&av);
	printk(KERN_INFO"Read the value %d\n",atomic_read(&av));
	atomic_sub(10,&av);
	printk(KERN_INFO"Read the value %d\n",atomic_read(&av));
	atomic_inc(&av);
	printk(KERN_INFO"Read the value %d\n",atomic_read(&av));
	atomic_dec(&av);
	printk(KERN_INFO"Read the value %d\n",atomic_read(&av));
	return 0;
}

static void __exit stop(void)
{
printk(KERN_INFO"Bye\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
