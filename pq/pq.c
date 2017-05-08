#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "pq.h"


static void pq_bubble_down(struct pq *pq);
static void pq_bubble_up(struct pq *pq);
static void pq_resize(struct pq *pq);
static void pq_swap(struct pq *pq, int i, int j);



void pq_init(struct pq *pq, int (*cmp_v)(const void *v1, const void *v2),
		void (*free_v)(void *v))
{
	pq->size = 0;
	pq->max_size = PQ_INIT_MAX_SIZE;
	pq->cmp_v = cmp_v;
	pq->free_v = free_v;
	if (!(pq->arr = malloc(sizeof(pq->arr[0]) * pq->max_size)))
		fprintf(stderr, "pq malloc() failure");

}

struct pq *pq_new(int (*cmp_v)(const void *v1, const void *v2),
		void (*free_v)(void *v))
{
	struct pq *pq;
	if (!(pq = malloc(sizeof(*pq))))
		fprintf(stderr, "pq malloc() failure");
	pq_init(pq, cmp_v, free_v);
	return pq;
}

void pq_free(struct pq *pq)
{
	free(pq->arr);
}

void pq_free_all(struct pq *pq)
{
	int i;
	for (i=0; i<pq->size; i++)
		pq->free_v(pq->arr[i]);
	free(pq->arr);
}

void pq_enq(struct pq *pq, void *v)
{
	if (pq->size == pq->max_size)
		pq_resize(pq);
	pq->arr[pq->size++] = v;
	pq_bubble_up(pq);
}

static void pq_resize(struct pq *pq)
{
	pq->max_size *= PQ_GROWTH_FACTOR;
	if (!(pq->arr = realloc(pq->arr, sizeof(pq->arr[0]) * pq->max_size)))
		fprintf(stderr, "pq malloc() failure");
}

static void pq_bubble_up(struct pq *pq)
{
	int cur, par;
	cur = pq->size - 1;
	while (cur > 0) {
		par = (cur-1)/2;
		if (pq->cmp_v(pq->arr[cur], pq->arr[par]) <= 0)
			return;
		pq_swap(pq, cur, par);
		cur = par;
	}
}

void *pq_deq(struct pq *pq)
{
	void *ret;
	if (!pq->size)
		return NULL;
	ret = pq->arr[0];
	pq->arr[0] = pq->arr[--pq->size];
	pq_bubble_down(pq);
	return ret;
}

static void pq_bubble_down(struct pq *pq)
{
	int cur, left, right, lrmax;
	cur = 0;
	while (1) {
		lrmax = cur;
		left = cur * 2 + 1;
		right = left + 1;
		if (left < pq->size &&
			pq->cmp_v(pq->arr[left], pq->arr[lrmax]) > 0)
			lrmax = left;
		if (right < pq->size &&
			pq->cmp_v(pq->arr[right], pq->arr[lrmax]) > 0)
			lrmax = right;
		if (lrmax == cur)
			return;
		pq_swap(pq, cur, lrmax);
		cur = lrmax;
	}
}

void *pq_peek(struct pq *pq)
{
	if (!pq->size)
		return NULL;
	return pq->arr[0];
}

static void pq_swap(struct pq *pq, int i, int j)
{
	void *temp;
	temp = pq->arr[i];
	pq->arr[i] = pq->arr[j];
	pq->arr[j] = temp;
}
