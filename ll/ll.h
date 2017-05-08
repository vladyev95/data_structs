#ifndef LL_H
#define LL_H

#include <stdlib.h>
#include <stdbool.h>


struct ll_node {
	void *v;
	struct ll_node *next, *prev;
};

struct ll {
	struct ll_node *head, *tail;
	size_t size;
	int (*cmp)(const void *v1, const void *v2);
	void (*free_v)(void *v);
};

void ll_init(struct ll *ll, int (*cmp)(const void *v1, const void *v2),
		void (*free_v)(void *v));


struct ll *ll_new(int (*cmp)(const void *v1, const void *v2),
		void (*free_v)(void *v));

void ll_free(struct ll *ll);
void ll_free_all(struct ll *ll);

bool ll_contains(const struct ll *ll, const void *val);

void ll_add_head(struct ll *ll, void *val);
void ll_add_tail(struct ll *ll, void *val);

void *ll_rm_head(struct ll *ll);
void *ll_rm_tail(struct ll *ll);

void ll_print(const struct ll *ll, void (*print)(const void *val));

#endif
