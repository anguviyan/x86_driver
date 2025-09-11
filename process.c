#include <linux/kernel.h>
#include <linux/module.h>
/*verify by sched/signal.h*/
static int __init start(void)
{
	struct task_struct *task;
	for_each_process(task){
		printk(KERN_INFO"The process name is %s and Process id is %d\n",task->comm,task->pid);
	};
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
