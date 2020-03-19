#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/proc_fs.h>

#define CDRIVER_PROC_NAME "cdriver_proc"

static struct device *devp;
struct cdev *cdevp;
static dev_t cdevno;
static int cdriver_major;
static int cdriver_minor;
static struct class *cdriver_class;
struct proc_dir_entry *cdriver_proc = NULL;

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

ssize_t cdriver_proc_read(struct file *filp, char __user *user_buf, size_t count, loff_t *ppos)
{
	printk("%s\n", __func__);

	return 0;
}

ssize_t cdriver_proc_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *ppos)
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

static const struct file_operations cdriver_proc_fops =
{
	.owner = THIS_MODULE,
	.read  = cdriver_proc_read,
	.write = cdriver_proc_write,
};

static int __init cdriver_init(void)
{
	int ret = 0;

	ret = alloc_chrdev_region(&cdevno, 0, 1, "cdriver_demo");
	if (ret < 0) {
		printk("alloc_chrdev_region failed!\n");
		return -1;
	}

	cdriver_major = MAJOR(cdevno);
	cdriver_minor = MINOR(cdevno);
	printk("major = %d, minor = %d\n", cdriver_major, cdriver_minor);

	cdevp = cdev_alloc();
	if (!cdevp) {
		printk("cdev_alloc failed!\n");
		goto err_cdev_alloc;
	}

	cdevp->owner = THIS_MODULE;
	cdevp->ops   = &cdriver_fops;

	cdev_add(cdevp, cdevno, 1);

	cdriver_class = class_create(THIS_MODULE, "cdriver_class");
	if (!cdriver_class) {
		printk("class_create failed!\n");
		goto err_class_create;
	}

	devp = device_create(cdriver_class, NULL, cdevno, NULL, "cdriver_dev");
	if (!devp) {
		printk("device_create failed!\n");
		goto err_device_create;
	}

	cdriver_proc = proc_create(CDRIVER_PROC_NAME, 0666, NULL, &cdriver_proc_fops);

	printk("cdriver demo module init!\n");
	return 0;

err_device_create:
	class_destroy(cdriver_class);
err_class_create:
	cdev_del(cdevp);
err_cdev_alloc:
	unregister_chrdev_region(cdevno, 1);
	return -1;
}

static void __exit cdriver_exit(void)
{
	device_destroy(cdriver_class, cdevno);
	class_destroy(cdriver_class);
	cdev_del(cdevp);
	unregister_chrdev_region(cdevno, 1);
	if (cdriver_proc)
		remove_proc_entry(CDRIVER_PROC_NAME, NULL);

	printk("cdriver demo module exit!\n");

	return ;
}

module_init(cdriver_init);
module_exit(cdriver_exit);

MODULE_LICENSE("GPL");
