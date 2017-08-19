#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "q.h"


void q_init(struct q *q, void (*free)(void *))
{
	q->size = q->front = q->back = 0;
	q->max_size = Q_INIT_MAX_SIZE;
	q->free = free;
	if (!(q->arr = malloc(sizeof(q->arr[0]) * Q_INIT_MAX_SIZE)))
		fprintf(stderr, "q_init() malloc() FAILURE\n");
}

struct q *q_new(void (*free_v)(void *v))
{
	struct q *q;
	if (!(q = malloc(sizeof(*q))))
		fprintf(stderr, "q_new() malloc() FAILURE\n");
	q_init(q, free_v);
	return q;
}



static void q_free_elems(struct q *q);

void q_free(struct q *q, int flags)
{
	if (flags && Q_FREE_ELEMS)
		q_free_elems(q);
	free(q->arr);
	if (flags & Q_FREE_PTR)
		free(q);
}

static void q_free_elems(struct q *q)
{
	int i;
	if (q->back < q->front) {
		for (i=0; i<=q->back; i++)
			q->free(q->arr[i]);
		for (i=q->front; i<q->max_size; i++)
			q->free(q->arr[i]);
	} else {
		for (i=q->front; i<=q->back && i<q->max_size; i++)
			q->free(q->arr[i]);
	}
}




static void q_resize(struct q *q, int new_max);

void q_enq(struct q *q, void *elem)
{
	if (q->size == q->max_size)
		q_resize(q, q->max_size * Q_GROWTH_FACTOR);

	if (q->size) {
		q->back = (q->back + 1) % q->max_size;
		q->arr[q->back] = elem;
	} else {
		q->front = q->back = 0;
		q->arr[0] = elem;
	}
	q->size++;
}

static void q_transfer_mem(void **new_arr, struct q *q);

static void q_resize(struct q *q, int new_max)
{
	void **new_arr;
	if (!(new_arr = malloc(sizeof(q->arr[0]) * new_max)))
		fprintf(stderr, "q_resize() malloc() FAILURE\n");
	
	q_transfer_mem(new_arr, q);

	free(q->arr);
	q->arr = new_arr;
	q->max_size = new_max;
	q->front = 0;
	q->back = q->size - 1;
}

static void q_transfer_mem(void **new_arr, struct q *q)
{
	if (q->back < q->front) {
		memcpy(new_arr, &q->arr[q->front], 
			(q->max_size - q->front) * sizeof(q->arr[0]));
		memcpy(&new_arr[q->max_size - q->front], q->arr,
			(q->back + 1) * sizeof(q->arr[0]));
	} else {
		memcpy(new_arr, &q->arr[q->front],
			(q->back - q->front + 1) * sizeof(q->arr[0]));
	}
}

static void *q_deq_has_elems(struct q *q);

void *q_deq(struct q *q)
{
	return q->size ? q_deq_has_elems(q) : NULL;
}

static void *q_deq_has_elems(struct q *q)
{
	void *ret;
	ret = q->arr[q->front];
	q->front = (q->front + 1) % q->max_size;
	q->size--;
	
	if (!q->size)
		q->front = q->back = 0;
	else if (q->max_size / Q_TRUNCATE_FACTOR >= Q_INIT_MAX_SIZE && 
			q->size < q->max_size / Q_TRUNCATE_THRESHOLD)
		q_resize(q, q->max_size / Q_TRUNCATE_FACTOR);

	return ret;
}



const void *q_peek(struct q *q)
{
	return q->size ? q->arr[q->front] : NULL;
}
