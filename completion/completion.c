#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>

#include <linux/slab.h>		//kmalloc kfree
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/completion.h>

struct completion_ctx {
	struct completion done1;
	struct completion done2;
	int               thread_flags;
};

struct completion_ctx *demo_completion;
static int wait_time = 10;

static int thread_func1(void *arg)
{
	if (wait_for_completion_timeout(&demo_completion->done1, wait_time * HZ)) {
		printk("The semaphore is actively released\n");
	} else {
		printk("wait_for_completion_timeout timeout\n");
		complete(&demo_completion->done1);
	}

	while (demo_completion->thread_flags) {
		wait_for_completion(&demo_completion->done1);
		printk("this is demo completion thread1\n");
		msleep(5000);
		complete(&demo_completion->done2);
	}

	printk("demo completion thread1 exit\n");
	return 0;
}

static int thread_func2(void *arg)
{
	while (demo_completion->thread_flags) {
		wait_for_completion(&demo_completion->done2);
		printk("this is demo completion thread2\n");
		msleep(1000);
		complete(&demo_completion->done1);
	}

	printk("demo completion thread2 exit\n");
	return 0;
}

static int __init demo_init(void)
{
	demo_completion = kmalloc(sizeof(*demo_completion), GFP_KERNEL);
	if (demo_completion == NULL) {
		printk("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err;
	}
	demo_completion->thread_flags = 1;

	init_completion(&demo_completion->done1);
	init_completion(&demo_completion->done2);
	kthread_run(thread_func1, NULL, "demo completion thread1");
	kthread_run(thread_func2, NULL, "demo completion thread2");

	return 0;

err:
	if (demo_completion)
		kfree(demo_completion);

	return -1;
}

static void __exit demo_exit(void)
{
	demo_completion->thread_flags = 0;

	complete(&demo_completion->done1);
	complete(&demo_completion->done2);

	msleep(10000);

	kfree(demo_completion);
	return ;
}

module_init(demo_init);
module_exit(demo_exit);
module_param(wait_time, int, 0644);
MODULE_PARM_DESC(wait_time, "Work queue scheduling times");

MODULE_LICENSE("GPL");
