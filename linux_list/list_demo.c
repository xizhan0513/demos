#include <stdio.h>
#include <string.h>

#include "list.h"

#define ARRAY_LEN 32

struct demo_list_st {
	int value;
	struct list_head demo_list_node;
};

int main()
{
	int i = 0;
	struct demo_list_st arr[ARRAY_LEN];
	struct list_head    demo_list_head;

	INIT_LIST_HEAD(&demo_list_head);

	/* 堆栈：先进后出 */
	for (i = 0; i < ARRAY_LEN; i++) {
		arr[i].value = i;
		list_add(&arr[i].demo_list_node, &demo_list_head);
	}

	printf("stack:\n");
	while (!list_empty(&demo_list_head)) {
		struct demo_list_st *new = list_entry(demo_list_head.next, struct demo_list_st, demo_list_node);
		printf("%d ", new->value);
		list_del(demo_list_head.next);
	}
	printf("\n");

	memset(arr, 0, sizeof(arr));

	/* 队列：先进先出 */
	for (i = 0; i < ARRAY_LEN; i++) {
		arr[i].value = i;
		list_add_tail(&arr[i].demo_list_node, &demo_list_head);
	}

	printf("queue:\n");
	while (!list_empty(&demo_list_head)) {
		struct demo_list_st *new = list_entry(demo_list_head.next, struct demo_list_st, demo_list_node);
		printf("%d ", new->value);
		list_del(demo_list_head.next);
	}
	printf("\n");

	return 0;
}
