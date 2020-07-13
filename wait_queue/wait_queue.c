#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/kthread.h>

static int flags = false;
static struct task_struct *demo_task;
wait_queue_head_t wqh;

/* 只是一个简单的demo，wait queue有很多接口，这里只实现基本的用法 */
static int demo_kthread_func(void *data)
{
	printk("wait_event start!\n");
	wait_event(wqh, flags == true);
	printk("wait_event done!\n");

	return 0;
}

static int __init demo_init(void)
{
	init_waitqueue_head(&wqh);

	demo_task = kthread_run(demo_kthread_func, NULL, "demo_kthread");
	if (IS_ERR(demo_task)) {
		printk("kthread_run failed!\n");
		return -1;
	}

	printk("Init done!\n");

	msleep(5000);
	flags = true;
	wake_up(&wqh);

	return 0;
}

static void __exit demo_exit(void)
{
	int ret = 0;

	if (!IS_ERR(demo_task)) {
		ret = kthread_stop(demo_task);
		printk("demo kthread has run %ds\n", ret);
	}

	printk("Exit done!\n");

	return ;
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
