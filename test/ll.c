#include <stdlib.h>
#include <stdio.h>
#include "ll.h"


void ll_init(struct ll *ll, int (*cmp)(const void *v1, const void *b2),
		void (*free_v)(void *v))
{
	ll->size = 0;
	ll->cmp = cmp;
	ll->free_v = free_v;
	ll->head = ll->tail = NULL;
}

struct ll *ll_new(int (*cmp)(const void *v1, const void *v2),
		void (*free_v)(void *v))
{
	struct ll *ll;
	if (!(ll = malloc(sizeof(*ll))))
		fprintf(stderr, "ll malloc() failure");
	ll_init(ll, cmp, free_v);
	return ll;
}

void ll_free(struct ll *ll)
{
	struct ll_node *walk, *next;
	for (walk = ll->head; walk; walk = next) {
		next = walk->next;
		free(walk);
	}
}

void ll_free_all(struct ll *ll)
{
	struct ll_node *walk, *next;
	for (walk = ll->head; walk; walk = next) {
		next = walk->next;
		ll->free_v(walk->v);
		free(walk);
	}
}

bool ll_contains(const struct ll *ll, const void *v)
{
	struct ll_node *walk;
	for (walk = ll->head; walk && ll->cmp(walk->v, v); walk = walk->next) ;
	return walk ? walk->v : NULL;
}

void ll_add_head(struct ll *ll, void *v)
{
	struct ll_node *new;
	if (!(new = malloc(sizeof(*new))))
		fprintf(stderr, "ll malloc() failure");
	new->v = v;
	if (!ll->size) {
		ll->head = ll->tail = new;
		new->next = new->prev = NULL;
	} else {
		new->next = ll->head;
		new->prev = NULL;
		ll->head->prev = new;
		ll->head = new;
	}
	ll->size++;
}

void ll_add_tail(struct ll *ll, void *v)
{
	struct ll_node *new;
	if (!(new = malloc(sizeof(*new))))
		fprintf(stderr, "ll malloc() failure");
	new->v = v;
	if (!ll->size) {
		ll->head = ll->tail = new;
		new->next = new->prev = NULL;
	} else {
		new->prev = ll->tail;
		new->next = NULL;
		ll->tail->next = new;
		ll->tail = new;
	}
	ll->size++;
}

void *ll_rm_head(struct ll *ll)
{
	void *ret;
	struct ll_node *old;
	if (!ll->size)
		return NULL;
	ret = ll->head->v;
	if (ll->size == 1) {
		free(ll->head);
		ll->head = ll->tail = NULL;
	} else {
		old = ll->head;
		ll->head = ll->head->next;
		ll->head->prev = NULL;
		free(old);
	}
	ll->size--;
	return ret;
}

void *ll_rm_tail(struct ll *ll)
{
	void *ret;
	struct ll_node *old;
	if (!ll->size)
		return NULL;
	ret = ll->tail->v;
	if (ll->size == 1) {
		free(ll->tail);
		ll->head = ll->tail = NULL;
	} else {
		old = ll->tail;
		ll->tail = ll->tail->prev;
		ll->tail->next = NULL;
		free(old);
	}
	ll->size--;
	return ret;
}

void ll_print(const struct ll *ll, void (*print)(const void *val))
{
	struct ll_node *walk;
	for (walk = ll->head; walk; walk = walk->next)
		print(walk->v);
}
