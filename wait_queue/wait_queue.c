#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/kthread.h>

static int wait_flags = false;
static int thread_exit_flags = false;
static struct task_struct *demo_task;
wait_queue_head_t wqh;

/* 只是一个简单的demo，wait queue有很多接口，这里只实现基本的用法 */
static int demo_kthread_func(void *data)
{
	printk("wait_event start!\n");
	wait_event(wqh, wait_flags == true);
	printk("wait_event done!\n");

	thread_exit_flags = true;

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

	return 0;
}

static void __exit demo_exit(void)
{
	int ret = 0;

	wait_flags = true;
	wake_up(&wqh);

	if (!IS_ERR(demo_task) && (thread_exit_flags == false)) {
		ret = kthread_stop(demo_task);
		printk("kthread_stop, ret = %d\n", ret);
	}

	printk("Exit done!\n");

	return ;
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");

/* 以下来自osal对wait_queue的封装 */
typedef int (*osal_wait_cond_func_t)(const void *param);
typedef struct osal_wait {
    void *wait;
} osal_wait_t;

unsigned long osal_msecs_to_jiffies(const unsigned int m)
{
    return msecs_to_jiffies(m);
}
EXPORT_SYMBOL(osal_msecs_to_jiffies);

int osal_wait_init(osal_wait_t *wait)
{
    wait_queue_head_t *wq = NULL;
    if (wait == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    wq = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t), GFP_ATOMIC);
    if (wq == NULL) {
        printk("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    init_waitqueue_head(wq);
    wait->wait = wq;
    return 0;
}
EXPORT_SYMBOL(osal_wait_init);

/* 如果在调用此接口前， func(param)返回true，则该接口不阻塞 */
int osal_wait_interruptible(osal_wait_t *wait, osal_wait_cond_func_t func, void *param)
{
    wait_queue_head_t *wq = NULL;
    DEFINE_WAIT(__wait);
    long ret = 0;
    int condition = 0;

    if (wait == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return -1;
    }
    prepare_to_wait(wq, &__wait, TASK_INTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (func != NULL) {
        condition = func(param);
    }

    if (!condition) {
        if (!signal_pending(current)) {
            schedule();
        }
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
        }
    }

    finish_wait(wq, &__wait);
    return ret;
}
EXPORT_SYMBOL(osal_wait_interruptible);

int osal_wait_uninterruptible(osal_wait_t *wait, osal_wait_cond_func_t func, void *param)
{
    wait_queue_head_t *wq = NULL;
    DEFINE_WAIT(__wait);
    long ret = 0;
    int condition = 0;

    if (wait == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return -1;
    }
    prepare_to_wait(wq, &__wait, TASK_UNINTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (func != NULL) {
        condition = func(param);
    }

    if (!condition) {
        schedule();
    }

    finish_wait(wq, &__wait);
    return ret;
}
EXPORT_SYMBOL(osal_wait_uninterruptible);

int osal_wait_timeout_interruptible(osal_wait_t *wait, osal_wait_cond_func_t func, void *param, unsigned long ms)
{
    wait_queue_head_t *wq = NULL;
    DEFINE_WAIT(__wait);
    long ret = ms;
    int condition = 0;

    if (wait == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return -1;
    }
    prepare_to_wait(wq, &__wait, TASK_INTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (func != NULL) {
        condition = func(param);
    }

    if (!condition) {
        if (!signal_pending(current)) {
            ret = schedule_timeout(msecs_to_jiffies(ret));
            ret = jiffies_to_msecs(ret);
        }
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
        }
    }

    finish_wait(wq, &__wait);

    return ret;
}

EXPORT_SYMBOL(osal_wait_timeout_interruptible);

int osal_wait_timeout_uninterruptible(osal_wait_t *wait, osal_wait_cond_func_t func, void *param, unsigned long ms)
{
    wait_queue_head_t *wq = NULL;
    DEFINE_WAIT(__wait);
    long ret = ms;
    int condition = 0;

    if (wait == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return -1;
    }
    prepare_to_wait(wq, &__wait, TASK_UNINTERRUPTIBLE);
    /* if wakeup the queue brefore prepare_to_wait, the func will return true. And will not go to schedule */
    if (func != NULL) {
        condition = func(param);
    }

    if (!condition) {
        ret = schedule_timeout(msecs_to_jiffies(ret));
        ret = jiffies_to_msecs(ret);
    }

    finish_wait(wq, &__wait);

    return ret;
}
EXPORT_SYMBOL(osal_wait_timeout_uninterruptible);

void osal_wakeup(osal_wait_t *wait)
{
    wait_queue_head_t *wq = NULL;

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return;
    }
    wake_up_all(wq);
}
EXPORT_SYMBOL(osal_wakeup);
void osal_wait_destory(osal_wait_t *wait)
{
    wait_queue_head_t *wq = NULL;

    wq = (wait_queue_head_t *)(wait->wait);
    if (wq == NULL) {
        printk("%s - wait->wait is NULL!\n", __FUNCTION__);
        return;
    }
    kfree(wq);
    wait->wait = NULL;
}
EXPORT_SYMBOL(osal_wait_destory);