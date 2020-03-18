#ifndef __BLOCK_ALLOCER_H__
#define __BLOCK_ALLOCER_H__

struct node {
	struct node *next;
};

typedef struct demo_pool_t {
	unsigned int      block_size;
	unsigned int      block_number;
	char             *buffer;
	char             *buffer_end;
	int               max_need_count;
	int               alloc_count;
	int               free_count;
	struct node      *chunk;
} demo_pool_t;

demo_pool_t *demo_pool_create(unsigned int block_size, unsigned int block_number, char *buffer);
void *demo_pool_alloc(demo_pool_t *pool, unsigned int size);
void demo_pool_free(demo_pool_t *pool, void *buffer);
void demo_pool_destory(demo_pool_t *pool);

#endif
