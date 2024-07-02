#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>

static int __init start(void)
{
	struct task_struct *task;
	for_each_process(task){
		pr_info("The Process name is %s and Process id is %d\n",task->comm,task->pid);
	};
	return 0;
}

static void __exit stop(void)
{
	pr_info("Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
