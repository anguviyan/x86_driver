#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/current.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>

#define KERN_BUF_SZ	512
#define DRV_CHAR_DRIVER "My_Char_Drv"
#define DRV_CLS_DRIVER "My_Drv_cls"
#define DRV_DEV_DRIVER "My_Drv_dev"

static dev_t first;
static struct cdev *vfs;
static struct class *drv_cls;
static struct device *drv_dev;

static int minor = 0;
static int count = 1;
static int inuse = 0;
static char *kbuf;
static int aval_kern_buf_sz = 0;

static int open (struct inode *inode, struct file *filp)
{
	inuse++;
	printk(KERN_INFO"Driver open is invoked by these many times %d\n",inuse);
	printk(KERN_INFO"Major no %d and Minor no %d\n",imajor(inode),iminor(inode));
	printk(KERN_INFO"Process name %s and Process id %d\n",current->comm, current->pid);
	printk(KERN_INFO"The refcount is %d\n",module_refcount(THIS_MODULE));
	return 0;
}	

static ssize_t write (struct file *filp, const char __user *wr_usr_buf, size_t wr_usr_buf_sz, loff_t *kern_buf_ptr_pos)
{
	printk(KERN_INFO"User requested size of memory for %zu\n",wr_usr_buf_sz);
	printk(KERN_INFO"kernel pointer position is %lld\n",*kern_buf_ptr_pos);

	int req_wr_kern_buf_sz,cur_kern_buf_ptr_pos;

	aval_kern_buf_sz = KERN_BUF_SZ - *kern_buf_ptr_pos;

	if(wr_usr_buf_sz < aval_kern_buf_sz)
	{
		req_wr_kern_buf_sz = wr_usr_buf_sz;
	}
	else
	{
		req_wr_kern_buf_sz = aval_kern_buf_sz;
	}

	if(req_wr_kern_buf_sz == 0)
	{
		printk(KERN_ERR"Memory reached Maximum\n");
		return -ENOSPC;
	}

	cur_kern_buf_ptr_pos = req_wr_kern_buf_sz - copy_from_user(kbuf+(*kern_buf_ptr_pos),wr_usr_buf,req_wr_kern_buf_sz);
	*kern_buf_ptr_pos +=	cur_kern_buf_ptr_pos;

	printk(KERN_INFO"Current kernel pointer position is %lld\n",*kern_buf_ptr_pos);

	return cur_kern_buf_ptr_pos;
}

static ssize_t read (struct file *filp, char __user *rd_usr_buf, size_t rd_usr_buf_sz, loff_t *kern_buf_ptr_pos)
{
	printk(KERN_INFO"User requested size of memory for %zu\n",rd_usr_buf_sz);
	printk(KERN_INFO"kernel pointer position is %lld\n",*kern_buf_ptr_pos);

	int req_rd_kern_buf_sz,cur_kern_buf_ptr_pos;

	aval_kern_buf_sz = KERN_BUF_SZ - *kern_buf_ptr_pos;

	if(rd_usr_buf_sz < aval_kern_buf_sz)
	{
		req_rd_kern_buf_sz = rd_usr_buf_sz;
	}
	else
	{
		req_rd_kern_buf_sz = aval_kern_buf_sz;
	}

	if(req_rd_kern_buf_sz == 0)
	{
		printk(KERN_ERR"Memory reached Maximum\n");
		return -ENOSPC;
	}

	cur_kern_buf_ptr_pos = req_rd_kern_buf_sz - copy_to_user(rd_usr_buf,kbuf+(*kern_buf_ptr_pos),req_rd_kern_buf_sz);

	*kern_buf_ptr_pos += cur_kern_buf_ptr_pos;
	
	printk(KERN_INFO"Current kernel pointer position is %lld\n",*kern_buf_ptr_pos);

	return cur_kern_buf_ptr_pos;
}

static int close (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"Driver is close the file operations\n");
	return 0;
}

static struct file_operations f_ops = {
	.owner	=	THIS_MODULE,
	.open	=	open,
	.write	=	write,
	.read	=	read,
	.release= 	close,
};

static int __init start(void)
{
	int ret;

	if(alloc_chrdev_region(&first, minor, count, DRV_CHAR_DRIVER) < 0)
	{
		printk(KERN_INFO"Driver is not register in the kernel\n");
		return -EINVAL;
	}
	printk(KERN_INFO"Driver is register in the kernel with %s\n",DRV_CHAR_DRIVER);
	printk(KERN_INFO"Major no %d and Minor no %d\n",MAJOR(first),MINOR(first));
	printk(KERN_INFO"Process name is %s and Process id is %d\n",current->comm,current->pid);

	kbuf = (char*)kzalloc(KERN_BUF_SZ,GFP_KERNEL);

	vfs = cdev_alloc();
	cdev_init(vfs,&f_ops);
	if(cdev_add(vfs,first,count) < 0)
	{
		printk(KERN_ERR"Driver is not registered in the VFS\n");
		return -EINVAL;
	}

	drv_cls = class_create(DRV_CLS_DRIVER);
	if(IS_ERR(drv_cls))
	{
		printk(KERN_ERR"Driver class is not created in the kernel\n");
		ret = PTR_ERR(drv_cls);
		goto drv_del;
	}
	printk(KERN_INFO"Driver class is created in the kernel with the name %s\n",DRV_CLS_DRIVER);

	drv_dev = device_create(drv_cls, NULL, first,NULL, "%s", DRV_DEV_DRIVER);
	if(IS_ERR(drv_dev))
	{
		printk(KERN_ERR"Driver device is not created in the kernel\n");
		ret = PTR_ERR(drv_dev);
		goto drv_dev;
	}
	printk(KERN_INFO"Drive device is created in the kernel with the name %s\n",DRV_DEV_DRIVER);

	return 0;

drv_del:
	class_destroy(drv_cls);
drv_dev:
	device_destroy(drv_cls,first);

	return ret;
}

static void __exit stop(void)
{
	device_destroy(drv_cls,first);
	printk(KERN_INFO"Driver is removed in the device from the kernel\n");
	class_destroy(drv_cls);
	printk(KERN_INFO"Driver is removed the class from the kernel\n");
	cdev_del(vfs);
	printk(KERN_INFO"Driver is not register in the VFS\n");
	kfree(kbuf);
	printk(KERN_INFO"Kernel memory is free\n");
	unregister_chrdev_region(first, count);
	printk(KERN_INFO"Driver is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
