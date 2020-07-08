#include <linux/init.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>

static struct task_struct *demo_task;
static char data[] = "test for kthread";

static int demo_kthread_func(void *data)
{
	int time_count = 30;

	while (!kthread_should_stop() && (time_count > 0)) {
		printk(KERN_INFO "PID: %d; NAEM: %s\n", current->pid, current->comm);
		printk(KERN_INFO "times: %d; data: %s\n", time_count--, (char *)data);
		msleep(1000);
	}

	return 0;
}

static int __init demo_init(void)
{
	demo_task = kthread_run(demo_kthread_func, (void *)data, "demo_kthread");
	if (IS_ERR(demo_task)) {
		printk("kthread_run failed!\n");
		return -1;
	}

	printk("Init done!\n");

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
