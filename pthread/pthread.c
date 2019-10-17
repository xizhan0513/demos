#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include <signal.h>

struct pthread_ctx {
	char *str;
};

unsigned int pthread_exit_flags;
struct pthread_ctx *demo_pthread;
static char data[] = "test for pthread";
static char pthread_exit_code[] = "pthread exit success";

/* 线程函数 */
void *pthread_func1(void *arg)
{
	/* 将非分离的线程设置为分离线程(返回当前线程的标识符) */
	//pthread_detach(pthread_self());

	/* 把当前线程的执行权出让给另一个有相同或更高优先级的线程 */
	//sched_yield();

	/* 给指定线程发送信号，若信号值为0，检测线程是否存在 */
#if 0
	int ret = pthread_kill(pthread_self(), 0);
	if (ret == ESRCH)
		printf("Thread has been terminated\n");
	else if (ret == EINVAL)
		printf("Invalid signal\n");
	else
		printf("Thread exists\n");
#endif

	while (pthread_exit_flags) {
		printf("I'am pthread %lu\n", pthread_self());
		sleep(1);
	}

	pthread_exit(pthread_exit_code);
}

void *pthread_func2(void *arg)
{
	while (pthread_exit_flags) {
		printf("I'am pthread %lu\n", pthread_self());
		sleep(1);
	}

	pthread_exit(pthread_exit_code);
}

int main()
{
	int ret = 0;
	char *pthread_status = NULL;
	pthread_t pid1, pid2;

	pthread_exit_flags = 1;

	demo_pthread = (struct pthread_ctx*)malloc(sizeof(struct pthread_ctx));
	if (demo_pthread == NULL) {
		printf("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err;
	}
	demo_pthread->str = data;

	/* 创建线程 */
	ret = pthread_create(&pid1, NULL, pthread_func1, demo_pthread);
	if (ret != 0) {
		printf("pthread create failed.\n");
		goto err;
	}

	ret = pthread_create(&pid2, NULL, pthread_func2, demo_pthread);
	if (ret != 0) {
		printf("pthread create failed.\n");
		goto err;
	}

	/* 比较线程 */
	ret = pthread_equal(pid1, pid2);
	if (ret) {
		printf("Two threads are equal\n");
	} else {
		printf("Two threads are not equal\n");
	}

	/* 等待线程结束 */
	ret = pthread_join(pid1, (void **)&pthread_status);
	printf("pthread_join return = %d\n", ret);
	if (pthread_status)
		printf("pthread exit status: %s\n", pthread_status);

	ret = pthread_join(pid2, (void **)&pthread_status);
	printf("pthread_join return = %d\n", ret);
	if (pthread_status)
		printf("pthread exit status: %s\n", pthread_status);

err:
	if (demo_pthread)
		free(demo_pthread);

	return 0;
}
