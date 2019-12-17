#include <linux/init.h>
#include <linux/module.h>

#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/delay.h>

int exit_flags;
int number;
struct semaphore sema_id1;
struct semaphore sema_id2;
struct semaphore sema_exit1;
struct semaphore sema_exit2;

static int thread_func1(void *arg)
{
	while (exit_flags != 0) {
		down(&sema_id1);
		printk("i am thread 1, number = %d\n", --number);
		msleep(1000);
		up(&sema_id2);
	}

	up(&sema_exit1);
	return 0;
}

static int thread_func2(void *arg)
{
	int ret = down_trylock(&sema_id2);
	if (ret == 0) {
		printk("sema_id2 get success\n");
	} else {
		printk("sema_id2 get failed\n");
	}

	while (exit_flags != 0) {
		down(&sema_id2);
		printk("i am thread 2, number = %d\n", ++number);
		msleep(1000);
		up(&sema_id1);
	}

	up(&sema_exit2);
	return 0;
}

static int __init demo_init(void)
{
	exit_flags = 1;
	number = 2;

	sema_init(&sema_id1, 1);
	sema_init(&sema_id2, 0);
	sema_init(&sema_exit1, 0);
	sema_init(&sema_exit2, 0);

	kthread_run(thread_func1, NULL, "demo completion thread1");
	kthread_run(thread_func2, NULL, "demo completion thread2");

	return 0;
}

static void __exit demo_exit(void)
{
	exit_flags = 0;
	down(&sema_exit1);
	down(&sema_exit2);

	return ;
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
