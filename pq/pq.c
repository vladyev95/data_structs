#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

void pq_init(struct pq *pq, int (*cmp)(const void *, const void *),
		void (*free)(void *))
{
	pq->size = 0;
	pq->max_size = PQ_INIT_MAX_SIZE;
	pq->cmp = cmp;
	pq->free = free;
	if (!(pq->arr = malloc(sizeof(pq->arr[0]) * pq->max_size)))
		fprintf(stderr, "pq_init() malloc() FAILURE\n");

}

struct pq *pq_new(int (*cmp)(const void *, const void *),
		void (*free)(void *))
{
	struct pq *pq;
	if (!(pq = malloc(sizeof(*pq))))
		fprintf(stderr, "pq_new() malloc() FAILURE\n");
	pq_init(pq, cmp, free);
	return pq;
}



static void pq_free_elems(struct pq *pq);

void pq_free(struct pq *pq, int flags)
{
	if (flags & PQ_FREE_ELEMS)
		pq_free_elems(pq);
	free(pq->arr);
	if (flags & PQ_FREE_PTR)
		free(pq);
}

static void pq_free_elems(struct pq *pq)
{
	int i;
	for (i=0; i<pq->size; i++)
		pq->free(pq->arr[i]);
}



static void pq_resize(struct pq *pq, int new_max);
static void pq_bubble_up(struct pq *pq);

void pq_enq(struct pq *pq, void *elem)
{
	if (pq->size == pq->max_size)
		pq_resize(pq, pq->max_size * PQ_GROWTH_FACTOR);
	pq->arr[pq->size++] = elem;
	pq_bubble_up(pq);
}


static void pq_resize(struct pq *pq, int new_max)
{
	if (!(pq->arr = realloc(pq->arr, new_max * sizeof(pq->arr[0]))))
		fprintf(stderr, "pq_resize() realloc() FAILURE\n");
	pq->max_size = new_max;
}


static void pq_bubble_up(struct pq *pq)
{
	int idx, par;
	void *temp;
	idx = pq->size - 1;
	par = (idx - 1) / 2;
	while (idx && pq->cmp(pq->arr[idx], pq->arr[par]) > 0) {
		temp = pq->arr[par];
		pq->arr[par] = pq->arr[idx];
		pq->arr[idx] = temp;
		idx = par;
		par = (idx - 1) / 2;
	}
}




static void *pq_deq_has_elems(struct pq *pq);
static void pq_bubble_down(struct pq *pq);

void *pq_deq(struct pq *pq)
{
	return pq->size ? pq_deq_has_elems(pq) : NULL;
}



static void *pq_deq_has_elems(struct pq *pq)
{
	void *ret;
	ret = pq->arr[0];
	pq->arr[0] = pq->arr[--pq->size];
	pq_bubble_down(pq);
	if (pq->max_size / PQ_TRUNCATE_FACTOR >= PQ_INIT_MAX_SIZE && 
			pq->size < pq->max_size / PQ_TRUNCATE_THRESHOLD)
		pq_resize(pq, pq->max_size / PQ_TRUNCATE_FACTOR);
	return ret;
}


static int pq_lrmax(struct pq *pq, int l, int r);
static void pq_swap(struct pq *pq, int idx1, int idx2);

static void pq_bubble_down(struct pq *pq)
{
	int idx, l, r, lrmax;
	idx = 0;
	l = idx * 2 + 1;
	r = l + 1;
	lrmax = pq_lrmax(pq, l, r);
	while (lrmax && pq->cmp(pq->arr[lrmax], pq->arr[idx]) > 0) {
		pq_swap(pq, idx, lrmax);
		idx = lrmax;
		l = idx * 2 + 1;
		r = l + 1;
		lrmax = pq_lrmax(pq, l, r);
	}
}

static int pq_lrmax(struct pq *pq, int l, int r)
{
	if (l >= pq->size && r >= pq->size)
		return 0;
	else if (l < pq->size && r >= pq->size)
		return l;
	else
		return pq->cmp(pq->arr[l], pq->arr[r]) >= 0 ? l: r;
}

static void pq_swap(struct pq *pq, int idx1, int idx2)
{
	void *temp;
	temp = pq->arr[idx1];
	pq->arr[idx1] = pq->arr[idx2];
	pq->arr[idx2] = temp;
}


const void *pq_peek(struct pq *pq)
{
	return pq->size ? pq->arr[0] : NULL;
}
