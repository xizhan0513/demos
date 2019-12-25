#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char client_msg[] = "hello server, i am client!";

int main()
{
	char buf[32];
	int client_len = 0;
	int client_sockfd = -1;
	struct sockaddr_in client_addr;

	memset(buf, 0, sizeof(buf));

	client_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (client_sockfd == -1) {
		perror("socket");
		return -1;
	}

	client_addr.sin_family      = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port        = htons(9736);

	client_len = sizeof(client_addr);
	if (sendto(client_sockfd, client_msg, sizeof(client_msg), 0, (struct sockaddr *)&client_addr,\
				client_len) < 0) {
		perror("write");
		goto end;
	}

	if (recvfrom(client_sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr,\
				&client_len) < 0) {
		perror("read");
		goto end;
	}
	printf("client: recv msg = %s\n", buf);

end:
	close(client_sockfd);

	return 0;
}
