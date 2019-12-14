#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int exit_flags;

void signal_handler()
{
	printf("i am server signal handler\n");
	exit_flags = 0;
	return ;
}

int main()
{
	int fd = 0;
	char str_fd[8];
	pid_t pid = getpid();

	fd = open("server.pid", O_WRONLY);
	if (fd < 0) {
		perror("open:");
		return -1;
	}

	memset(str_fd, 0, sizeof(str_fd));
	sprintf(str_fd, "%d", pid);
	if (write(fd, str_fd, sizeof(str_fd)) <= 0) {
		printf("write error\n");
		close(fd);
		return -1;
	}

	signal(SIGUSR1, signal_handler);

	exit_flags = 1;
	while(exit_flags) {
		sleep(1);
	}

	close(fd);
	return 0;
}
