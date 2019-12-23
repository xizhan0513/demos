#include <stdio.h>
#include <string.h>
#include "queue.h"

#define ARR_SIZE 10

struct demo {
	int demo_value;
	TAILQ_ENTRY(demo) demo_link;
};

struct demo_info {
	TAILQ_HEAD(demohead, demo) demo_head;
};

struct demo_info demoinfo;
struct demo_info *demoinfop;
struct demo demoarr[ARR_SIZE];
struct demo *demo_temp;

static void init_tailq(void)
{
	int i;
	demoinfop = &demoinfo;

	memset(demoinfop, 0, sizeof(*demoinfop));
	memset(&demoarr[0], 0, sizeof(demoarr));

	TAILQ_INIT(&demoinfop->demo_head);

	for (i = 0; i < ARR_SIZE; i++) {
		demoarr[i].demo_value = i;
		TAILQ_INSERT_TAIL(&demoinfop->demo_head, &demoarr[i], demo_link);
	}

	return ;
}

static int tailq_empty(void)
{
	if (TAILQ_EMPTY(&demoinfop->demo_head)) {
		printf("tailq is empty!\n");
		return -1;
	} else {
		printf("tailq is not empty!\n");
		return 0;
	}
}

static void forward_traversal(void)
{
	printf("%s:\n", __func__);

	TAILQ_FOREACH(demo_temp, &demoinfop->demo_head, demo_link)
		printf("%d ", demo_temp->demo_value);
	printf("\n");

	return ;
}

static void reverse_traversal(void)
{
	printf("%s:\n", __func__);

	TAILQ_FOREACH_REVERSE(demo_temp, &demoinfop->demo_head, demohead, demo_link)
		printf("%d ", demo_temp->demo_value);
	printf("\n");

	return ;
}

static void remove_tailq()
{
	TAILQ_FOREACH_REVERSE(demo_temp, &demoinfop->demo_head, demohead, demo_link)
		TAILQ_REMOVE(&demoinfop->demo_head, demo_temp, demo_link);

	return ;
}

int main ()
{
	init_tailq();

	if (tailq_empty())
		return -1;

	forward_traversal();

	reverse_traversal();

	remove_tailq();

	if (!tailq_empty())
		return -1;

	return 0;
}
