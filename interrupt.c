#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 28;
module_param(irq,int,S_IRUSR);
MODULE_PARM_DESC(irq,"This will be displayed in the /var/log/kern.log");

static int counter = 0;
static int my_id = 100;

static irqreturn_t my_handler(int irq,void *dev_id)
{
	counter++;
	printk(KERN_INFO"These many times interrupt is occured in the kernel %d\n",counter);
	return IRQ_NONE; /*include/linux/irqreturn.h*/
}

static int __init start(void)
{
	if(request_irq(irq,my_handler, IRQF_SHARED,"My_interrupt",&my_id) < 0)
	{
		printk(KERN_INFO"This interrupt is not generated in the kernel\n");
		return -EINVAL;                                                       /*/uapi/asm-generic/errno-base.h*/
	}
	printk(KERN_INFO"The interrupt is occured in the kernel by the id %d\n",irq);
	return 0;
}

static void __exit stop(void)
{
	free_irq(irq, &my_id);
	printk(KERN_INFO"INterrupt is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
