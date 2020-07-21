#include <linux/init.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>

static int kthread_exit_flags = false;
static struct task_struct *demo_task;
static char data[] = "test for kthread";

static int demo_kthread_func(void *data)
{
	int time_count = 10;

	while (!kthread_should_stop() && (time_count > 0)) {
		printk(KERN_INFO "PID: %d; NAEM: %s\n", current->pid, current->comm);
		printk(KERN_INFO "times: %d; data: %s\n", time_count--, (char *)data);
		msleep(1000);
	}

	kthread_exit_flags = true;

	return 13;
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

/*
	1、调用kthread_stop()之后，首先唤醒线程，然后一直阻塞，直到线程退出(所以不要在线程中调用kthread_stop()，不然会死锁)。
	2、调用kthread_stop()之后，设置线程的退出标记，kthread_should_stop()会返回true(所以要在线程中调用kthread_should_stop()进行判断)。
	3、一般线程退出后，调用do_exit()释放一些系统资源；需要调用kthread_stop()彻底释放内核线程所占用的系统资源。
	注：调用kthread_stop()时，线程必须还没有退出，不然会出现Oops的问题。
*/
static void __exit demo_exit(void)
{
	int ret = 0;

	if (!IS_ERR(demo_task) && (kthread_exit_flags == false)) {
		ret = kthread_stop(demo_task);
		printk("kthread_stop, ret = %d\n", ret);
	}

	printk("Exit done!\n");

	return ;
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
