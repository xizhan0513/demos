#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>

/* 当前线程调度策略 */
static void show_thread_policy(pthread_attr_t *attr)
{
	int rs, policy;
	/* 获取当前线程的调度策略 */
	rs = pthread_attr_getschedpolicy(attr, &policy);
	if (rs != 0) {
		printf("pthread attr getschedpolicy failed\n");
		return ;
	}

	switch (policy) {
		case SCHED_FIFO:
			printf("policy = SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf("policy = SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf("policy = SCHED_OTHER\n");
			break;
		default:
			printf("policy = UNKNOWN\n");
			break;
	}

	return ;
}

static void show_thread_priority(pthread_attr_t *attr, int policy)
{
	int priority;
	/* 获取该调度策略的最大优先级 */
	priority = sched_get_priority_max(policy);
	if (priority == -1) {
		printf("sched get priority max failed\n");
		return ;
	}
	printf("max priority = %d\n", priority);

	/* 获取该调度策略的最小优先级 */
	priority = sched_get_priority_min(policy);
	if (priority == -1) {
		printf("sched get priority min failed\n");
		return ;
	}
	printf("min priority = %d\n", priority);
}

static void show_thread_current_priority(pthread_attr_t *attr)
{
	int rs;
	struct sched_param param;

	/* 获取线程优先级 */
	rs = pthread_attr_getschedparam(attr, &param);
	if (rs != 0) {
		printf("pthread attr getschedparam failed\n");
		return ;
	}

	printf("current thread priotity = %d\n", param.__sched_priority);
	return ;
}

int main()
{
	int rs;
	pthread_attr_t attr;
	struct sched_param sched;

	/* 初始化一个线程对象的属性 */
	rs = pthread_attr_init(&attr);
	if (rs != 0) {
		printf("pthread attr init failed, errno = %d\n", rs);
		return -1;
	}

	printf("--- show SCHED_OTHER of priority ---\n");
	show_thread_priority(&attr, SCHED_OTHER);
	printf("--- show SCHED_FIFO of priority ---\n");
	show_thread_priority(&attr, SCHED_FIFO);
	printf("--- show SCHED_RR of priority ---\n");
	show_thread_priority(&attr, SCHED_RR);

	printf("show thread info:\n");
	show_thread_policy(&attr);
	show_thread_current_priority(&attr);

	/* 设置线程调度策略 */
	rs = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	if (rs != 0) {
		printf("pthread attr setschedpolicy failed\n");
		return -1;
	}
	sched.__sched_priority = 33;
	/* 设置线程优先级 */
	pthread_attr_setschedparam(&attr, &sched);

	printf("show thread info:\n");
	show_thread_policy(&attr);
	show_thread_current_priority(&attr);

	rs = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (rs != 0) {
		printf("pthread attr setschedpolicy failed\n");
		return -1;
	}
	sched.__sched_priority = 66;
	pthread_attr_setschedparam(&attr, &sched);

	printf("show thread info:\n");
	show_thread_policy(&attr);
	show_thread_current_priority(&attr);

	/* 注销一个线程对象 */
	rs = pthread_attr_destroy(&attr);
	if (rs != 0) {
		printf("pthread attr destroy failed, errno = %d\n", rs);
		return -1;
	}

	return 0;
}
