#include <linux/init.h>
#include <linux/module.h>

#include <linux/slab.h>		//kmalloc kfree
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

struct tasklet_ctx {
	struct tasklet_struct real_tasklet;
};

struct tasklet_ctx *demo_tasklet;
static int count = 10;
static char data[] = "test for tasklet";

static void demo_tasklet_func(unsigned long arg)
{
	printk(KERN_INFO "[tasklet]=> PID: %d; NAEM: %s\n", current->pid, current->comm);
	printk(KERN_INFO "[tasklet]=>data is: %s\n", (char*)arg);

	return ;
}

static int __init demo_init(void)
{
	demo_tasklet = kmalloc(sizeof(*demo_tasklet), GFP_KERNEL);
	if (demo_tasklet == NULL) {
		printk("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err;
	}

	tasklet_init(&demo_tasklet->real_tasklet, demo_tasklet_func, (unsigned long)data);

	while (count--) {
		msleep(5000);
		tasklet_schedule(&demo_tasklet->real_tasklet);
	}

	return 0;

err:
	if (demo_tasklet)
		kfree(demo_tasklet);

	return -1;
}

static void __exit demo_exit(void)
{
	tasklet_kill(&demo_tasklet->real_tasklet);
	kfree(demo_tasklet);
	return ;
}

module_init(demo_init);
module_exit(demo_exit);
module_param(count, int, 0644);
MODULE_PARM_DESC(count, "tasklet scheduling times");

MODULE_LICENSE("GPL");
