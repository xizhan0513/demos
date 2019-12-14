#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	int fd = 0;
	char str_fd[8];
	pid_t pid = 0;

	fd = open("server.pid", O_RDONLY);
	if (fd < 0) {
		perror("open:");
		return -1;
	}

	memset(str_fd, 0, sizeof(str_fd));
	if (read(fd, str_fd, sizeof(str_fd)) <= 0) {
		printf("read error\n");
		close(fd);
		return -1;
	}

	pid = atoi(str_fd);
	kill(pid, SIGUSR1);

	close(fd);
	return 0;
}
