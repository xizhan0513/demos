#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>

#define TEST_TIMES 100
#define EACH_OP_SIZE (64 * 1024)

int main(int argc, char *argv[])
{
	int fd, ret, i;
	void *buf = NULL;
	struct timeval start, end;
	unsigned int delta_ms;

	if (argc != 2) {
		printf("Usage: ./a.out write_filename\n");
		return -1;
	}

	buf = malloc(EACH_OP_SIZE);
	if (!buf) {
		printf("malloc failed\n");
		goto err;
	}

	fd = open((char *)argv[1], O_RDWR | O_CREAT, 666);
	if (fd < 0) {
		perror("open:\n");
		goto err;
	}

	for (i = 0; i < TEST_TIMES; i++) {
		gettimeofday(&start, NULL);
		ret = write(fd, buf, EACH_OP_SIZE);
		gettimeofday(&end, NULL);
		delta_ms = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
		printf("time : %u\n", delta_ms);
		if (ret != EACH_OP_SIZE)
			printf("write less, ret : %d\n", ret);
	}

	close(fd);
err:
	if (buf)
		free(buf);

	return 0;
}
