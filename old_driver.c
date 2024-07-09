#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/current.h>
#include <linux/slab.h>

#define MAJORNO 300
#define MINORNO 0
#define CHAR_NAME "sri_dev"
#define MAX_SIZE 2048

static dev_t first;
static unsigned int count = 1;
static struct cdev *mydev;
static int inuse = 0;
static char *kbuf;

static int open(struct inode *inode, struct file *filp)
{
	if(inuse)
	{
		pr_info("This device is busy\n");
		return -ENOSPC;
	}
	inuse++;
	pr_info("The Major no is %d and the minor no is %d\n",imajor(inode),iminor(inode));
	pr_info("The Process name is %s and process id is %d\n",current->comm,current->pid);
	pr_info("The Refno = %d\n",module_refcount(THIS_MODULE));
	return 0;
}

static ssize_t read(struct file *filp, char __user *usr_buf_dat, size_t usr_buf_sz, loff_t *kptr)
{
	int aval_mem_sz;
	int dat_read_buf;
	int cur_pos_kptr;

	pr_info("The size of the buffer requested by the user is %ld\n",usr_buf_sz);
	pr_info("The position of the kernel pointer is %lld\n",*kptr);
	aval_mem_sz = MAX_SIZE - *kptr;

	if(usr_buf_sz < aval_mem_sz)
	{
		dat_read_buf = usr_buf_sz;
	}
	else
	{
		dat_read_buf = aval_mem_sz;
	}

	if(dat_read_buf == 0)
	{
		pr_info("There is no memory in the buf\n");
		return -ENOSPC;
	}

	cur_pos_kptr = dat_read_buf - copy_from_user(kbuf + (*kptr),usr_buf_dat,dat_read_buf);
	*kptr += cur_pos_kptr;

	pr_info("The current position of the kernel pointer is %lld\n",*kptr);

	return cur_pos_kptr;
}

static ssize_t write(struct file *filp, const char __user *usr_buf_dat, size_t usr_buf_sz, loff_t *kptr)
{
	int aval_mem_sz;
	int dat_writ_buf;
	int cur_pos_kptr;

	pr_info("The size of the buffer requested by the user is %ld\n",usr_buf_sz);
	pr_info("The current position of the kernel pointer is %lld\n",*kptr);

	aval_mem_sz = MAX_SIZE - *kptr;

	if (usr_buf_sz < aval_mem_sz)
	{
		dat_writ_buf = usr_buf_sz;
	}
	else
	{
		dat_writ_buf = aval_mem_sz;
	}

	if(dat_writ_buf == 0)
	{
		pr_info("There is no memory in the buf\n");
		return -ENOSPC;
	}

	cur_pos_kptr = dat_writ_buf - copy_to_user((void*)usr_buf_dat,kbuf+(*kptr),dat_writ_buf);
	*kptr += cur_pos_kptr;
	
	pr_info("The current position of the kernel pointer %lld\n",*kptr);

	return cur_pos_kptr;
}

static int close(struct inode *inode, struct file *filp)
{
	inuse--;
	pr_info("This is sucessfully removed from the kernel\n");
	return 0;
}

static struct file_operations f_ops = {
	.owner	=	THIS_MODULE,
	.open	=	open,
	.release=	close,
	.read	=	read,
	.write	=	write,
};

static int __init start(void)
{
	first = MKDEV(MAJORNO,MINORNO);
	if(register_chrdev_region(first,count,CHAR_NAME) < 0)
	{
		pr_info("This driver is not register in the kernel\n");
		return -ENOSPC;
	}
	mydev = cdev_alloc();
	cdev_init(mydev,&f_ops);
	if(cdev_add(mydev,first,count) < 0)
	{
		pr_info("This driver is not created the entry in the character device driver\n");
		unregister_chrdev_region(first,count);
		return -ENOSPC;
	}
	kbuf = (char*)kzalloc(MAX_SIZE,GFP_KERNEL);
	pr_info("The driver %s is sucessfully installed in the kernel\n",CHAR_NAME);
	pr_info("The Major no %d and the Minor no %d\n",MAJOR(first),MINOR(first));
	pr_info("The process name is %s and Process id %d\n",current->comm,current->pid);
	return 0;
}

static void __exit stop(void)
{
	kfree(kbuf);
	cdev_del(mydev);
	unregister_chrdev_region(first,count);
	pr_info("This driver is successfully removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
