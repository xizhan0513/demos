#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>

#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/slab.h> //kmalloc kfree

#include <linux/sched.h>
#include <linux/delay.h>

int count = 10;
static char data[] = "test for work queue";

struct work_ctx {
	struct work_struct real_work;
	char *str;
	int arg;
};

struct work_ctx *demo_work;

static void demo_work_func(struct work_struct *work) {
	struct work_ctx *temp_work = container_of(work, struct work_ctx, real_work);
	printk(KERN_INFO "[work]=> PID: %d; NAEM: %s\n", current->pid, current->comm);
	printk(KERN_INFO "[work]=> sleep 1 seconds\n");
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(1 * HZ);
	printk(KERN_INFO "[work]=>data is: %d %s\n", temp_work->arg, temp_work->str);
}

static int __init demo_thread_init(void)
{
	demo_work = kmalloc(sizeof(*demo_work), GFP_KERNEL);
	if (demo_work == NULL) {
		printk("%s: malloc failed\n", __func__);
		goto err;
	}

	INIT_WORK(&demo_work->real_work, demo_work_func);
	demo_work->str = data;
	while (count--) {
		msleep(5000);
		demo_work->arg = count;
		schedule_work(&demo_work->real_work);
	}

	return 0;

err:
	if (demo_work)
		kfree(demo_work);

	return 0;
}

static void __exit demo_thread_exit(void)
{
	flush_work(&demo_work->real_work);
	kfree(demo_work);
	return ;
}

module_init(demo_thread_init);
module_exit(demo_thread_exit);
module_param(count, int, 0644);

MODULE_LICENSE("GPL");
