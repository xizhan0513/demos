#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_COUNT 10

int count;
sem_t sem_p;		/* 用于检测池是否满的信号量 */
sem_t sem_c;		/* 用于检测池是否空的信号量 */
sem_t sem_mutex;	/* 用于互斥的信号量，用来保护临界资源count */

void *producer(void *arg)
{
	while (1) {
		sem_wait(&sem_p);

		sem_wait(&sem_mutex);

		count++;
		printf("%s: count = %d\n", __func__, count);

		if (count < MAX_COUNT)
			sem_post(&sem_p);
		if (count == 1)
			sem_post(&sem_c);

		sem_post(&sem_mutex);
	}

	return NULL;
}

void *consumer(void *arg)
{
	while (1) {
		sem_wait(&sem_c);

		sem_wait(&sem_mutex);

		count--;
		printf("%s: count = %d\n", __func__, count);

		if (count > 0)
			sem_post(&sem_c);
		if (count == MAX_COUNT - 1)
			sem_post(&sem_p);

		sem_post(&sem_mutex);
	}

	return NULL;
}

int main()
{
	int ret;
	pthread_t pid_1, pid_2;

	count = 0;
	sem_init(&sem_p, 0, 1);
	sem_init(&sem_c, 0, 0);
	sem_init(&sem_mutex, 0, 1);

	ret = pthread_create(&pid_1, NULL, producer, NULL);
	if (ret != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	ret = pthread_create(&pid_2, NULL, consumer, NULL);
	if (ret != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	pthread_join(pid_1, NULL);
	pthread_join(pid_2, NULL);

	return 0;
}
