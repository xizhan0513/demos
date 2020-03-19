#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

struct cdev cdev;
static dev_t cdevno;
static int cdriver_major = 251;
static int cdriver_minor = 0;

int cdriver_open(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);

	return 0;
}

long cdriver_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("%s\n", __func__);

	return 0;
}

int cdriver_release(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);

	return 0;
}

static const struct file_operations cdriver_fops =
{
	.owner          = THIS_MODULE,
	.open           = cdriver_open,
	.unlocked_ioctl = cdriver_ioctl,
	.release        = cdriver_release,
};

static int __init cdriver_init(void)
{
	int ret = 0;

	cdevno = MKDEV(cdriver_major, cdriver_minor);
	ret = register_chrdev_region(cdevno, 1, "cdriver_demo");
	if (ret < 0) {
		printk("register_chrdev_region failed!\n");
		return -1;
	}

	cdev_init(&cdev, &cdriver_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops   = &cdriver_fops;

	cdev_add(&cdev, cdevno, 1);

	/* 需要手动mknod创建设备节点 */

	printk("cdriver demo module init!\n");
	return 0;
}

static void __exit cdriver_exit(void)
{
	cdev_del(&cdev),
	unregister_chrdev_region(cdevno, 1);
	printk("cdriver demo module exit!\n");

	return ;
}

module_init(cdriver_init);
module_exit(cdriver_exit);

MODULE_LICENSE("GPL");
