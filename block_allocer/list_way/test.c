#include <stdio.h>
#include <string.h>
#include "block_allocer.h"

int main()
{
	demo_block_t *buf;
	demo_pool_t *g_pool;

	g_pool = demo_pool_create(1024 * 1024, 512);
	if (!g_pool) {
		printf("demo_pool_create exec failed\n");
		return -1;
	}

	buf = demo_pool_alloc(g_pool);
	if (!buf) {
		printf("demo_pool_alloc exec failed\n");
	} else {
		printf("alloc: buf = %p\n", buf);
	}

	demo_pool_free(g_pool, buf);

	demo_pool_destory(g_pool);

	return 0;
}
