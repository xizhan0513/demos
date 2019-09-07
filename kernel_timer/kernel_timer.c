#include <linux/init.h>
#include <linux/module.h>

#include <linux/slab.h>		//kmalloc kfree
#include <linux/sched.h>
#include <linux/delay.h>

struct timer_ctx {
	struct timer_list real_timer;
};

static struct timer_ctx *demo_timer;
static unsigned int expire = 5;
static char data[] = "test for kernel timer";

void demo_timer_func(unsigned long arg)
{
	printk(KERN_INFO "[timer]=> PID: %d; NAEM: %s\n", current->pid, current->comm);
	printk(KERN_INFO "[timer]=> %s\n", (char*)arg);

	mod_timer(&demo_timer->real_timer, (jiffies + expire * HZ));

	return ;
}

static int __init demo_init(void)
{
	demo_timer = kmalloc(sizeof(*demo_timer), GFP_KERNEL);
	if (demo_timer == NULL) {
		printk("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err;
	}

	init_timer(&demo_timer->real_timer);

	demo_timer->real_timer.expires  = jiffies + expire * HZ;
	demo_timer->real_timer.function = demo_timer_func;
	demo_timer->real_timer.data     = (unsigned long)data;

	add_timer(&demo_timer->real_timer);

	return 0;

err:
	if (demo_timer)
		kfree(demo_timer);
	return -1;
}

static void __exit demo_exit(void)
{
	int ret;
	/* 主动删除定时器，超时后会自动删除定时器；当还没超时，该函数返回1，已经超时返回0 */
	ret = del_timer(&demo_timer->real_timer);
	printk("%s: del_timer return %d\n", __func__, ret);

	kfree(demo_timer);
	return ;
}

module_init(demo_init);
module_exit(demo_exit);
module_param(expire, int, 0644);
MODULE_PARM_DESC(expire, "kernel timer expire time(jiffies)");

MODULE_LICENSE("GPL");
