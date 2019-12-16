#include "named_semaphore_process.h"

int main()
{
	char ch;
	int shmid;
	key_t key = 1000;
	char *shm, *s;
	sem_t *sem_id;

	sem_id = sem_open(SEM_NAME, O_CREAT, 0644, 1);
	if (sem_id == SEM_FAILED) {
		perror("sem_open");
		sem_unlink(SEM_NAME);
		return -1;
	}

	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
	if (shmid < 0) {
		perror("shmid");
		sem_close(sem_id);
		sem_unlink(SEM_NAME);
		return -1;
	}

	shm = shmat(shmid, NULL, 0);
	s = shm;

	for (ch = 'A'; ch <= 'Z'; ch++) {
		sem_wait(sem_id);
		*s++ = ch;
		sem_post(sem_id);
	}

	while (*shm != '*') {
		sleep(1);
	}

	sem_close(sem_id);
	sem_unlink(SEM_NAME);
	shmctl(shmid, IPC_RMID, 0);

	return 0;
}
