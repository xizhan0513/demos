#include "named_semaphore_process.h"

int main()
{
	char ch;
	int shmid;
	key_t key = 1000;
	char *shm, *s;
	sem_t *sem_id;

	sem_id = sem_open(SEM_NAME, 0, 0644, 1);
	if (sem_id == SEM_FAILED) {
		perror("sem_open");
		return -1;
	}

	shmid = shmget(key, SHMSZ, 0666);
	if (shmid < 0) {
		perror("shmid");
		sem_close(sem_id);
		return -1;
	}

	shm = shmat(shmid, NULL, 0);
	s = shm;

	for (s = shm; *s != '\0'; s++) {
		sem_wait(sem_id);
		putchar(*s);
		sem_post(sem_id);
	}

	*shm = '*';

	sem_close(sem_id);
	shmctl(shmid, IPC_RMID, 0);

	return 0;
}
