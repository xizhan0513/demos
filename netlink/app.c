#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#define NETLINK_TEST 30
#define MSG_LEN      125
#define MAX_PLOAD    125

typedef struct _user_msg_info
{
	struct nlmsghdr hdr;
	char msg[MSG_LEN];
} user_msg_info;

int main()
{
	int nlfd;
	int ret;
	user_msg_info u_info;
	socklen_t len;
	struct nlmsghdr *nlh = NULL;
	struct sockaddr_nl saddr, daddr;
	char *umsg = "hello, netlink!!!";

	/* 创建netlink socket */
	nlfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
	if (nlfd == -1) {
		perror("create socket failed");
		return -1;
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.nl_family = AF_NETLINK;
	saddr.nl_pid = 100;	//端口号
	saddr.nl_groups = 0;

	if (bind(nlfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
		perror("bind failed");
		goto err1;
	}

	memset(&daddr, 0, sizeof(daddr));
	daddr.nl_family = AF_NETLINK;
	daddr.nl_pid = 0;	//to kernel
	daddr.nl_groups = 0;

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
	if (nlh == NULL) {
		printf("%s: malloc failed in %d lines!\n", __func__, __LINE__);
		goto err1;
	}
	memset(nlh, 0, sizeof(struct nlmsghdr));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = 0;
	nlh->nlmsg_seq = 0;
	nlh->nlmsg_pid = saddr.nl_pid;	//self port

	memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));

	ret = sendto(nlfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&daddr, sizeof(struct sockaddr_nl));
	if (!ret) {
		perror("sendto failed");
		goto err2;
	}
	printf("send to kernel: %s\n", umsg);

	memset(&u_info, 0, sizeof(u_info));
	len = sizeof(struct sockaddr_nl);
	ret = recvfrom(nlfd, &u_info, sizeof(user_msg_info), 0, (struct sockaddr *)&daddr, &len);
	if (!ret) {
		perror("recv from kernel error");
		goto err2;
	}
	printf("recv from kernel: %s\n", u_info.msg);

err2:
	free(nlh);
err1:
	close(nlfd);
	return 0;
}
