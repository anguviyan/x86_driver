#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/atomic.h>

static int __init start(void)
{
	atomic_t av = ATOMIC_INIT(10);
	pr_info("The value of atomic is %d\n",atomic_read(&av));
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
