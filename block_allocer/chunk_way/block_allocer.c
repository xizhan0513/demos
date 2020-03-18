#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block_allocer.h"

demo_pool_t *demo_pool_create(unsigned int block_size, unsigned int block_number, char *buffer)
{
	int i;
	int mem_size = block_size * block_number;
	demo_pool_t *pool;
	struct node *chunk;

	if (buffer == NULL) {
		pool = (demo_pool_t *)malloc(sizeof(demo_pool_t) + mem_size);
		if (!pool) {
			return NULL;
		}

		pool->buffer = (char *)pool + sizeof(demo_pool_t);
	} else {
		pool = (demo_pool_t *)malloc(sizeof(demo_pool_t));
		if (!pool) {
			return NULL;
		}

		pool->buffer = buffer;
	}

	pool->block_number   = block_number;
	pool->block_size     = block_size;
	pool->buffer_end     = pool->buffer + mem_size;
	pool->max_need_count = 0;
	pool->alloc_count    = 0;
	pool->free_count     = 0;

	chunk = (struct node *)pool->buffer;
	pool->chunk = chunk;

	for (i = 0; i < block_number - 1; i++) {
		chunk->next = (struct node *)((char *)chunk + block_size);
		chunk = chunk->next;
	}

	chunk->next = NULL;

	return pool;
}

void *demo_pool_alloc(demo_pool_t *pool, unsigned int size)
{
	char *mem = NULL;

	if (size <= 0) {
		return NULL;
	}

	if (pool) {
		if (pool->chunk != NULL && size <= pool->block_size) {
			mem = (char *)pool->chunk;
			pool->chunk = pool->chunk->next;

			pool->alloc_count++;
			if (pool->alloc_count - pool->free_count > pool->max_need_count)
				pool->max_need_count = pool->alloc_count - pool->free_count;
		}
	}

	if (mem == NULL) {
		mem = malloc(size);
	}

	return mem;
}

void demo_pool_free(demo_pool_t *pool, void *buffer)
{
	struct node *chunk = (struct node *)buffer;
	char *mem = (char *)buffer;

	if (mem == NULL) {
		return ;
	}

	if (pool) {
		if ((char *)buffer >= pool->buffer && (char *)buffer < pool->buffer_end) {
			chunk->next = pool->chunk;
			pool->chunk = chunk;

			pool->free_count++;
		} else {
			free(buffer);
		}

		return ;
	}

	free(buffer);
	return ;
}

void demo_pool_destory(demo_pool_t *pool)
{
	if (pool) {
		free(pool);
	}

	return ;
}
