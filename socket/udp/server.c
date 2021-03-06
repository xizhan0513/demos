#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

char server_msg[] = "hello client, i am server!";

int main()
{
	int ret;
	char buf[32];
	int client_len = 0;
	int server_sockfd = -1;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	memset(buf, 0, sizeof(buf));

	server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_sockfd == -1) {
		perror("socket");
		return -1;
	}

	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port        = htons(9736);

	ret = bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		perror("bind");
		goto end;
	}

	client_len = sizeof(server_addr);
	if (recvfrom(server_sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr,\
				&client_len) < 0) {
		perror("read");
		goto end;
	}
	printf("server: recv msg = %s\n", buf);

	if (sendto(server_sockfd, server_msg, sizeof(server_msg), 0, (struct sockaddr *)&client_addr,\
				client_len) < 0) {
		perror("write");
		goto end;
	}

end:
	close(server_sockfd);

	return 0;
}
