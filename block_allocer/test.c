#include <stdio.h>
#include <string.h>
#include "block_allocer.h"

int main()
{
	char *buf;
	demo_pool *g_pool;

	g_pool = demo_pool_create(512, 64, NULL);
	if (!g_pool) {
		printf("demo_pool_create exec failed\n");
		return -1;
	}

	buf = demo_pool_alloc(g_pool, 256);
	if (!buf) {
		printf("demo_pool_alloc exec failed\n");
	} else {
		memset(buf, 1, 256);
		printf("alloc: buf = %p, buf@4 = %#x\n", buf, *((int *)buf));
	}

	demo_pool_free(g_pool, buf);
	printf("free: buf = %p, buf@4 = %#x\n", buf, *((int *)buf));

	demo_pool_destory(g_pool);

	return 0;
}
