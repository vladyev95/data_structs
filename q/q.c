#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "q.h"



void q_init(struct q *q, void (*free_v)(void *v))
{
	q->size = 0;
	q->front = q->back = NULL;
	q->free_v = free_v;
}

struct q *q_new(void (*free_v)(void *v))
{
	struct q *q;
	if (!(q = malloc(sizeof(*q))))
		fprintf(stderr, "q malloc() failure");
	q_init(q, free_v);
	return q;
}

void q_free(struct q *q)
{
	struct q_node *walk, *next;
	for (walk = q->front; walk; walk = next) {
		next = walk->next;
		free(walk);
	}
}

void q_free_all(struct q *q)
{
	struct q_node *walk, *next;
	for (walk = q->front; walk; walk = next) {
		next = walk->next;
		q->free_v(walk->v);
		free(walk);
	}
}

void *q_peek(struct q *q)
{
	return q->front ? q->front->v : NULL;
}

void q_enq(struct q *q, void *v)
{
	struct q_node *new;
	if (!(new = malloc(sizeof(*new))))
		fprintf(stderr, "q malloc() failure");
	new->v = v;
	if (!q->size) {
		q->front = q->back = new;
		new->next = NULL;
	} else {
		q->back->next = new;
		q->back = new;
		new->next = NULL;
	}
	q->size++;
}


void *q_deq(struct q *q)
{
	struct q_node *front;
	void *ret;
	if (!q->size)
		return NULL;
	front = q->front;
	ret = front->v;
	q->front = front->next;
	free(front);
	q->size--;
	if (!q->size)
		q->back = NULL;
	return ret;
}
