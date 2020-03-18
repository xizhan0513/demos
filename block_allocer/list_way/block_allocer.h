#ifndef __BLOCK_ALLOCER_H__
#define __BLOCK_ALLOCER_H__

#include "list.h"

#define ALIGNTO 4U
#define ALIGN4(len) ( ((len) + ALIGNTO - 1) & ~(ALIGNTO - 1))

typedef struct demo_block {
	struct list_head list_node;
	/* 可以添加一些描述block的成员变量 */
	unsigned char   *data[0];
} demo_block_t;

typedef struct demo_pool {
	char            *membase;
	unsigned int     memsize;
	struct list_head alloced_list_head;
	struct list_head free_list_head;
} demo_pool_t;

demo_pool_t *demo_pool_create(unsigned int mem_size, unsigned int block_size);
demo_block_t *demo_pool_alloc(demo_pool_t *pool);
void demo_pool_free(demo_pool_t *pool, demo_block_t *block);
void demo_pool_destory(demo_pool_t *pool);

#endif
