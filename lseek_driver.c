#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/current.h>
#include <linux/device.h>
#include <linux/slab.h>

#define CHAR_NAME "sri_dev"
#define MAX_SIZE 2048

static int minor = 0;
static int count = 1;
static int inuse = 0;
static char *kbuf;

static dev_t dev_num;
static struct cdev *mydev;
static struct class *cls;
static struct device *dev;

static int my_open(struct inode *inode, struct file *filp)
{
	if(inuse)
	{
		pr_info("This driver is busy now\n");
		return -EINVAL;
	}
	inuse++;
	pr_info("Major no %d : Minor no %d\n",imajor(inode),iminor(inode));
	pr_info("Process name %s : Process id %d\n",current->comm,current->pid);
	pr_info("The Ref_no = %d\n",module_refcount(THIS_MODULE));
	return 0;
}

static int my_close(struct inode *inode, struct file *filp)
{
	inuse--;
	pr_info("This is currently used by these many users %d\n",inuse);
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *usr_rd_dat, size_t usr_buf_sz, loff_t *kptr)
{
	int avl_mem_sz,dat_rd_mem,cur_pos_kptr;

	pr_info("The current position of the kernel buffer pointer is %lld\n",*kptr);
	pr_info("The user buffer requested by the user is %ld\n",usr_buf_sz);
	
	avl_mem_sz = MAX_SIZE - *kptr;
	
	if(usr_buf_sz < MAX_SIZE)
	{
		dat_rd_mem = usr_buf_sz;
	}
	else
	{
		dat_rd_mem = MAX_SIZE;
	}

	if(dat_rd_mem == 0)
	{
		pr_info("This device reached the end of the memory\n");
		return -ENOSPC;
	}

	cur_pos_kptr = dat_rd_mem - copy_to_user((void*)usr_rd_dat,kbuf+*kptr,dat_rd_mem);

	*kptr += cur_pos_kptr;

	pr_info("The current position of the kernel pointer is %lld\n",*kptr);

	return cur_pos_kptr;
}

static ssize_t my_write(struct file *filp, const char __user *usr_wr_dat, size_t usr_buf_sz, loff_t *kptr)
{
	int avl_mem_sz,cur_pos_kptr,dat_wr_mem;

	pr_info("The current position of the kernel pointer is %lld\n",*kptr);
	pr_info("The user requested kernel buffer size is %ld\n",usr_buf_sz);

	avl_mem_sz = MAX_SIZE - *kptr;

	if(usr_buf_sz < avl_mem_sz)
	{	
		dat_wr_mem = usr_buf_sz;
	}
	else
	{
		dat_wr_mem = avl_mem_sz;
	}

	if(dat_wr_mem == 0)
	{
		pr_info("The memory is reached the end\n");
		return -ENOSPC;
	}

	cur_pos_kptr = dat_wr_mem - copy_from_user(kbuf+(*kptr),usr_wr_dat,dat_wr_mem);

	*kptr += cur_pos_kptr;

	pr_info("The kernel pointer position is %lld\n",*kptr);
	return cur_pos_kptr;
}

static loff_t my_seek(struct file *filp, loff_t pos, int whence)
{
	loff_t tmp_pos;

	pr_info("The requested position is %lld\n",pos);

	switch(whence)
	{
		case 0: /*seek_set*/
			tmp_pos = pos;
			break;
		case 1: /*seek_cur*/
			tmp_pos = filp->f_pos + pos;
			break;
		case 2:/*seek_end*/
			tmp_pos = MAX_SIZE + pos;
			break;
		default:
			return -EINVAL;
	}

	tmp_pos = tmp_pos < MAX_SIZE ? tmp_pos : MAX_SIZE;
	tmp_pos = tmp_pos >= 0 ? tmp_pos : 0;

	filp->f_pos = tmp_pos;
	pr_info("seek is %ld\n",(long)tmp_pos);
	return tmp_pos;
}

static struct file_operations f_ops = {
	.owner	=	THIS_MODULE,
	.open	=	my_open,
	.release=	my_close,
	.read	=	my_read,
	.write	= 	my_write,
	.llseek =	my_seek,
};

static int __init start(void)
{
	if(alloc_chrdev_region(&dev_num,minor,count,CHAR_NAME) < 0)
	{
		pr_info("This device is not register in the kernel\n");
		return -EINVAL;
	}

	mydev = cdev_alloc();
	cdev_init(mydev,&f_ops);
	if(cdev_add(mydev,dev_num,count) < 0)
	{
		pr_info("This device is not creating the vfs character device driver for this device\n");
		cdev_del(mydev);
		unregister_chrdev_region(dev_num,count);
		return -EINVAL;
	}
	cls = class_create(THIS_MODULE,"MY_DEVICE");
	dev = device_create(cls,NULL,dev_num,NULL,"%s",CHAR_NAME);
	kbuf = (char*)kzalloc(MAX_SIZE,GFP_KERNEL);
	pr_info("This driver is register and installed with the %s\n",CHAR_NAME);
	pr_info("MAjor no %d : Minor no %d\n",MAJOR(dev_num),MINOR(dev_num));
	pr_info("The process name %s:Process id %d\n",current->comm,current->pid);
	return 0;
}

static void __exit stop(void)
{
	device_destroy(cls,dev_num);
	class_destroy(cls);
	cdev_del(mydev);
	unregister_chrdev_region(dev_num,count);
	pr_info("This driver is sucessfully removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
