#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int number;
sem_t sem_id1;
sem_t sem_id2;

void *pthread_func1(void *arg)
{
	while (1) {
		sem_wait(&sem_id1);
		printf("i am thread 1, number = %d\n", --number);
		sleep(1);
		sem_post(&sem_id2);
	}

	return NULL;
}

void *pthread_func2(void *arg)
{
	int sval = 0;
	sem_trywait(&sem_id2);
	sem_getvalue(&sem_id2, &sval);
	printf("now, sem_id2 = %d\n", sval);

	while (1) {
		sem_wait(&sem_id2);
		printf("i am thread 2, number = %d\n", ++number);
		sleep(1);
		sem_post(&sem_id1);
	}

	return NULL;
}

int main()
{
	int ret;
	pthread_t pid_1, pid_2;
	number = 2;

	sem_init(&sem_id1, 0, 1);
	sem_init(&sem_id2, 0, 0);

	ret = pthread_create(&pid_1, NULL, pthread_func1, NULL);
	if (ret != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	ret = pthread_create(&pid_2, NULL, pthread_func2, NULL);
	if (ret != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	pthread_join(pid_1, NULL);
	pthread_join(pid_1, NULL);

	return 0;
}
