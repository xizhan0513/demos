#include <linux/init.h>
#include <linux/module.h>

#include <linux/slab.h>		//kmalloc kfree
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

struct workqueue_ctx {
	struct work_struct real_work;
	char               *str;
};

struct workqueue_ctx *demo_workqueue;
static int count = 10;
static char data[] = "test for work queue";

static void demo_workqueue_func(struct work_struct *work)
{
	struct workqueue_ctx *temp_work = container_of(work, struct workqueue_ctx, real_work);
	printk(KERN_INFO "[work]=> PID: %d; NAEM: %s\n", current->pid, current->comm);
	printk(KERN_INFO "[work]=> sleep 1 seconds\n");
	/* 设置当前进程处于等待队列中，等待资源有效时唤醒 */
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(1 * HZ);
	printk(KERN_INFO "[work]=>data is: %s\n", temp_work->str);

	return ;
}

static int __init demo_init(void)
{
	demo_workqueue = kmalloc(sizeof(*demo_workqueue), GFP_KERNEL);
	if (demo_workqueue == NULL) {
		printk("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err;
	}

	INIT_WORK(&demo_workqueue->real_work, demo_workqueue_func);
	demo_workqueue->str = data;

	while (count--) {
		msleep(5000);
		schedule_work(&demo_workqueue->real_work);
	}

	return 0;

err:
	if (demo_workqueue)
		kfree(demo_workqueue);

	return -1;
}

static void __exit demo_exit(void)
{
	flush_work(&demo_workqueue->real_work);
	kfree(demo_workqueue);
	return ;
}

module_init(demo_init);
module_exit(demo_exit);
module_param(count, int, 0644);
MODULE_PARM_DESC(count, "Work queue scheduling times");

MODULE_LICENSE("GPL");
