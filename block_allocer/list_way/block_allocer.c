#include <stdio.h>
#include <stdlib.h>
#include "block_allocer.h"

demo_pool_t *demo_pool_create(unsigned int mem_size, unsigned int block_size)
{
	char *block_mem;
	unsigned int avail_mem_size, block_mem_size;
	demo_pool_t *pool;

	pool = (demo_pool_t *)malloc(sizeof(demo_pool_t));
	if (!pool) {
		return NULL;
	}

	pool->memsize = mem_size;
	pool->membase = (char *)malloc(pool->memsize);
	if (!pool->membase) {
		free(pool);
		return NULL;
	}

	INIT_LIST_HEAD(&pool->alloced_list_head);
	INIT_LIST_HEAD(&pool->free_list_head);

	block_mem      = pool->membase;
	avail_mem_size = pool->memsize;
	block_mem_size = ALIGN4(sizeof(demo_block_t) + block_size);

	while (avail_mem_size >= block_mem_size) {
		demo_block_t *block = (demo_block_t *)block_mem;

		list_add(&block->list_node, &pool->free_list_head);

		block_mem      += block_mem_size;
		avail_mem_size -= block_mem_size;
	}

	return pool;
}

demo_block_t *demo_pool_alloc(demo_pool_t *pool)
{
	if (!list_empty(&pool->free_list_head)) {
		demo_block_t *new;

		new = list_entry(pool->free_list_head.next, demo_block_t, list_node);
		list_del(pool->free_list_head.next);
		list_add(&new->list_node, &pool->alloced_list_head);

		return new;
	} else {
		return NULL;
	}
}

void demo_pool_free(demo_pool_t *pool, demo_block_t *block)
{
	list_del(&block->list_node);
	list_add(&block->list_node, &pool->free_list_head);

	return ;
}

void demo_pool_destory(demo_pool_t *pool)
{
	if (pool->membase) {
		free(pool->membase);
	}

	if (pool) {
		free(pool);
	}

	return ;
}
