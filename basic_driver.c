#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/current.h>

#define MAJORNO 300
#define MINORNO 0
#define CHAR_NAME "sri_devices"

static char *mydriver = "sridriver";
static dev_t first;
static unsigned int count = 1;
static struct cdev *mydev;
static int inuse = 0;

static int open(struct inode *inode, struct file *filp)
{
	if(inuse)
	{
		pr_info("This device is used by others\n");
		return -ENOSPC;
	}
	inuse++;
	pr_info("The Majorno is %d,Minorno is %d\n",imajor(inode),iminor(inode));
	pr_info("The process name is %s,Process id is %d\n",current->comm,current->pid);
	pr_info("The refcont = %d\n",module_refcount(THIS_MODULE));
	pr_info("open is called\n");
	return 0;
}

static ssize_t read(struct file *filp, char __user *usr_buf_dat, size_t usr_buf_sz, loff_t *kptr)
{
	pr_info("read is called\n");
	return 0;
}

static ssize_t write(struct file *filp, const char __user *usr_buf_dat, size_t usr_buf_sz, loff_t *kptr)
{
	pr_info("write is called\n");
	return 0;
}

static int close (struct inode *inode, struct file *filp) 
{
	pr_info("close is called\n");
	return 0;
}

static struct file_operations f_ops = {
	.owner	=	THIS_MODULE,
	.open	=	open,
	.release=	close,
	.write	=	write,
	.read	=	read,
};

static int __init start(void)
{
	pr_info("%s is installing in the kernel\n",mydriver);
	first = MKDEV(MAJORNO,MINORNO);
	if(register_chrdev_region(first,count,CHAR_NAME) < 0)
	{
		pr_info("This device is not register in the kernel\n");
		return -ENOSPC;
	}
	mydev = cdev_alloc();
	cdev_init(mydev,&f_ops);
	if(cdev_add(mydev,first,count) < 0)
	{
		pr_info("This is not register in the kernel\n");
		cdev_del(mydev);
		unregister_chrdev_region(first,count);
		return -ENOSPC;
	}
	pr_info("This driver is successfully installed in the kernel with %s\n",CHAR_NAME);
	pr_info("The Major no is %d,Minor no is %d\n",MAJOR(first),MINOR(first));
	pr_info("The process name is %s,Process id is %d\n",current->comm,current->pid);
	return 0;
}

static void __exit stop(void)
{
	cdev_del(mydev);
	unregister_chrdev_region(first,count);
	pr_info("This driver is successfully removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
